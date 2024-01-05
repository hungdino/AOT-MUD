#include "unp.h"
#include <sys/wait.h>

#define MAX_CLIENTS 2  // Define MAX_CLIENTS

void sig_chld(int signo) {
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

void chat_session(int sockfd1, int sockfd2, char* name1, char* name2) {
    ssize_t n;
    char buf[MAXLINE], sendbuf[MAXLINE];
    fd_set rset;
    int maxfdp1;
    int closed1 = 0, closed2 = 0;

    FD_ZERO(&rset);
    maxfdp1 = max(sockfd1, sockfd2) + 1;

    for (;;) {
        if (!closed1) FD_SET(sockfd1, &rset);
        if (!closed2) FD_SET(sockfd2, &rset);

        Select(maxfdp1, &rset, NULL, NULL, NULL);

        if (!closed1 && FD_ISSET(sockfd1, &rset)) {
            if ((n = read(sockfd1, buf, MAXLINE)) == 0) {
                snprintf(sendbuf, sizeof(sendbuf), "(%s) left the room. Press Ctrl+D to leave.\n", name1);
                Writen(sockfd2, sendbuf, strlen(sendbuf));
                closed1 = 1;
                continue;
            }
            buf[n] = 0;
            snprintf(sendbuf, sizeof(sendbuf), "%s", buf);
            Writen(sockfd2, sendbuf, strlen(sendbuf));
        }

        if (!closed2 && FD_ISSET(sockfd2, &rset)) {
            if ((n = read(sockfd2, buf, MAXLINE)) == 0) {
                snprintf(sendbuf, sizeof(sendbuf), "(%s) left the room. Press Ctrl+D to leave.\n", name2);
                Writen(sockfd1, sendbuf, strlen(sendbuf));
                closed2 = 1;
                continue;
            }
            buf[n] = 0;
            snprintf(sendbuf, sizeof(sendbuf), "%s", buf);
            Writen(sockfd1, sendbuf, strlen(sendbuf));
        }

        if (closed1 && !closed2) {
            if ((n = read(sockfd2, buf, MAXLINE)) == 0) {
                snprintf(sendbuf, sizeof(sendbuf), "(%s) left the room.\n", name2);
                Writen(sockfd1, sendbuf, strlen(sendbuf));
                break;
            }
        }

        if (closed2 && !closed1) {
            if ((n = read(sockfd1, buf, MAXLINE)) == 0) {
                snprintf(sendbuf, sizeof(sendbuf), "(%s) left the room.\n", name1);
                Writen(sockfd2, sendbuf, strlen(sendbuf));
                break;
            }
        }
    }
}

int main(int argc, char **argv) {
    int listenfd, connfd[MAX_CLIENTS];
    pid_t childpid;
    char client_name[MAX_CLIENTS][MAXLINE], buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;
    socklen_t clilen;

    Signal(SIGCHLD, sig_chld);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT + 3);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd[0] = Accept(listenfd, (SA *) &cliaddr, &clilen);
        Read(connfd[0], client_name[0], MAXLINE);
        Write(connfd[0], "You are the 1st user. Wait for the second one!\n", strlen("You are the 1st user. Wait for the second one!\n"));

        connfd[1] = Accept(listenfd, (SA *) &cliaddr, &clilen);
        Read(connfd[1], client_name[1], MAXLINE);
	
      snprintf(buf, sizeof(buf), "The second user is %s from %s\n", client_name[1], inet_ntoa(cliaddr.sin_addr));
       Write(connfd[0], buf, strlen(buf));
 

        snprintf(buf, sizeof(buf), "You are the 2nd user\nThe first user is %s from %s\n", client_name[0], inet_ntoa(cliaddr.sin_addr));
        Write(connfd[1], buf, strlen(buf));

        if ((childpid = Fork()) == 0) {
            Close(listenfd);
            chat_session(connfd[0], connfd[1], client_name[0], client_name[1]);
            exit(0);
        }

        Close(connfd[0]);
        Close(connfd[1]);
    }
}


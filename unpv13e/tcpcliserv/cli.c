#include	"unp.h"
#include  <string.h>
#include  <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>


char id[MAXLINE];



const char *ascii_art = 
"   _______ __________________ _______  _______  _           _______  _      \n"
"  (  ___  )\\__   __/\\__   __/(  ___  )(  ____ \\| \\    /\\   (  ___  )( (    /| \n"
"  | (   ) |   ) (      ) (   | (   ) || (    \\/|  \\  / /   | (   ) ||  \\  ( | \n"
"  | (___) |   | |      | |   | (___) || |      |  (_/ /    | |   | ||   \\ | | \n"
"  |  ___  |   | |      | |   |  ___  || |      |   _ (     | |   | || (\\ \\) |  \n"
"  | (   ) |   | |      | |   | (   ) || |      |  ( \\ \\    | |   | || | \\   | \n"
"  | )   ( |   | |      | |   | )   ( || (____/\\|  /  \\ \\   | (___) || )  \\  | \n"
"  |/     \\|   )_(      )_(   |/     \\|(_______/|_/    \\/   (_______)|/    )_) \n"
"\n"
"                 ___________________________ _______  _       \n"
"                 \\__   __/\\__   __/\\__   __/(  ___  )( (    /|\n"
"                    ) (      ) (      ) (   | (   ) ||  \\  ( |\n"
"                    | |      | |      | |   | (___) ||   \\ | |\n"
"                    | |      | |      | |   |  ___  || (\\ \\) |\n"
"                    | |      | |      | |   | (   ) || | \\   |\n"
"                    | |   ___) (___   | |   | )   ( || )  \\  |\n"
"                    )_(   \\_______/   )_(   |/     \\||/    )_)";


void clr_scr() {
	printf("\x1B[2J");
};

void set_scr() {		// set screen to 80 * 25 color mode
	printf("\x1B[=3h");
};

void xchg_data(FILE *fp, int sockfd) {
    int maxfdp1, stdineof, peer_exit, n;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    set_scr();
    clr_scr();
    Writen(sockfd, id, strlen(id));
    printf("sent: %s\n", id);
    readline(sockfd, recvline, MAXLINE);
    printf("recv: %s", recvline);
    readline(sockfd, recvline, MAXLINE);
    printf("recv: %s", recvline); 
    stdineof = 0;
    peer_exit = 0;
     clr_scr();
     printf("%s\n", ascii_art); 
    printf("choose 1 to join the game  or 2 to be spectator\n"); 
    
    clr_scr();
    for ( ; ; ) { 
        FD_ZERO(&rset);
        maxfdp1 = 0;
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
            maxfdp1 = fileno(fp);
        };  
        if (peer_exit == 0) {
            FD_SET(sockfd, &rset);
            if (sockfd > maxfdp1)
                maxfdp1 = sockfd;
        };  
        maxfdp1++;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rset)) {  /* socket is readable */
            n = read(sockfd, recvline, MAXLINE);
            if (n == 0) {
                if (stdineof == 1)
                    return;         /* normal termination */
                else {
                    printf("(End of input from the peer!)\n");
                    peer_exit = 1;
                };
            }
            else if (n > 0) {
                recvline[n] = '\0';
                if(recvline[0]=='5')
                {
                printf("game over\n");
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);
                }
                else if(recvline[0]=='6')
                {
                
                int random_number = (rand() % 4) + 1; 
           	sprintf(sendline, "%d\n", random_number); 
           	printf("send random num:%d\n",random_number);
           	Writen(sockfd, sendline, strlen(sendline)); 
                }
                else
                {
                printf("%s", recvline);
                }
            };
        }

        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
        	
            if (Fgets(sendline, MAXLINE, fp) == NULL) {
                if (peer_exit)
                    return;
                else {
                    printf("(leaving...)\n");
                    stdineof = 1;
                    Shutdown(sockfd, SHUT_WR);      /* send FIN */
                };
            }
            else {
                n = strlen(sendline);
                if (n > 0 && sendline[n-1] == '\n') {
                    sendline[n-1] = '\0'; // Remove the newline character
                }
                // Check if the input is one of the valid options
                if (strcmp(sendline, "1") == 0 || strcmp(sendline, "2") == 0 ||
                    strcmp(sendline, "3") == 0 || strcmp(sendline, "4") == 0|| strcmp(sendline, "5") == 0||strcmp(sendline, "6") == 0) {
                    sendline[n-1]='\n';
                    n = strlen(sendline);
                    Writen(sockfd, sendline, n);
                } else {
                    printf("Invalid input. Please enter 1, 2, 3, or 4.\n");
                }
                
            };
        }
    }
};


int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 3)
		err_quit("usage: tcpcli <IPaddress> <ID>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT+3);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	strcpy(id, argv[2]);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	xchg_data(stdin, sockfd);		/* do it all */

	exit(0);
}

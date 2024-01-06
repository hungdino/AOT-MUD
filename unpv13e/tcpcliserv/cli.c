#include	"unp.h"
#include  <string.h>
#include  <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
char id[MAXLINE];



const char* ascii_art =
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

void setup_non_blocking_io() {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &newt);
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
void print_menu(int highlight, char* choices[], int n_choices, int start_row) {
    //printf("\033[%d;1H", start_row);
    printf("\033[G"); // 将光标移动到当前行的最左边
    printf("\033[K"); // 清除行内容

    for (int i = 0; i < n_choices; ++i) {
        if (i == highlight) {
            printf("\033[1;32m"); // 高亮选中的选项
        }
        printf("%s", choices[i]);
        printf("\033[0m"); // 重置颜色
        if (i < n_choices - 1) {
            printf("  "); // 选项间的空格
        }
    }
    fflush(stdout);
}

void xchg_data(FILE* fp, int sockfd) {
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
    printf("choose 1 to join the game  or 2 to be spectator\n");
    clr_scr();
    int highlight = 0;
    setup_non_blocking_io();
    for (; ; ) {
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
                if (recvline[0] == '5')
                {
                    printf("game over\n");
                    stdineof = 1;
                    Shutdown(sockfd, SHUT_WR);
                }
                else if (recvline[0] == '6')
                {

                    int random_number = (rand() % 4) + 1;
                    sprintf(sendline, "%d\n", random_number);
                    printf("send random num:%d\n", random_number);
                    Writen(sockfd, sendline, strlen(sendline));
                }
                else
                {
                    printf("a\n", recvline);
                    printf("%s\n", recvline);
                }
            };
        }
        char *choices[3][3] = {
        {"选项 A", "选项 B", "选项 C"},
        {"选项 A", "选项 B", "选项 C"},
        {"选项 A", "选项 B", "选项 C"}
    };
        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
            
            int n_choices = 3;
            int start_row = 2;
                
                char ch;
            	read(STDIN_FILENO, &ch, 1);
                switch (ch) {
                case 'a': // 左键
                
                    highlight = (highlight - 1 + n_choices) % n_choices;
                    //printf("a\n");
                    print_menu(highlight, choices[0], n_choices, start_row);
                    //printf("a2\n");
                    break;
                case 'd': // 右键
                    //printf("d\n");
                    highlight = (highlight + 1) % n_choices;
                    print_menu(highlight, choices[0], n_choices, start_row);
                    break;
                case 'q': // 退出
                    if (peer_exit)
                        return;
                    else {
                        printf("(leaving...)\n");
                        stdineof = 1;
                        Shutdown(sockfd, SHUT_WR);      /* send FIN */
                    };
                case 'e': // Enter键
                    int user_choice = highlight;
                    sprintf(sendline, "%d", user_choice);
                    n = strlen(sendline);
                    
                    Writen(sockfd, sendline, n);
                    printf("%s\n", sendline);
                }
            }
         
        
    }
};


int
main(int argc, char** argv)
{
    int					sockfd;
    struct sockaddr_in	servaddr;

    if (argc != 3)
        err_quit("usage: tcpcli <IPaddress> <ID>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT + 3);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    strcpy(id, argv[2]);

    Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));

    xchg_data(stdin, sockfd);		/* do it all */

    exit(0);
}

#include	"unp.h"
#include  <string.h>
#include  <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
char id[MAXLINE];

struct sockaddr_in	servaddr;
struct termios origt;
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
void set_blocking_echo() {
    struct termios tty;
    
    // 获取当前的终端设置
    tcgetattr(STDIN_FILENO, &tty);

    // 设置终端为阻塞模式并启用回显
    tty.c_lflag |= ICANON;
    tty.c_lflag |= ECHO;

    // 应用这些更改
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
void setup_non_blocking_io() {
    struct termios newt;

    // 获取当前的设置，并保存
    

    // 复制原始设置以进行修改
    newt = origt;

    // 修改设置为非阻塞和无回显
    newt.c_lflag &= ~(ICANON | ECHO);

    // 应用新设置
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
void restore_original_settings() {
    // 恢复原始的termios设置
    tcsetattr(STDIN_FILENO, TCSANOW, &origt);
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
char *choices[3][3] = {
        {"[1]", "[2]", "[3]"},
        {"A", "B", "C"},
        {"选项 A", "选项 B", "选项 C"}
    };
bool xchg_data(FILE* fp, int sockfd) {
    int maxfdp1, stdineof, peer_exit, n;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE], buff[MAXLINE],recvlinex[MAXLINE];
    char *found2 = NULL;
    set_scr();
    //clr_scr();
    size_t len = strlen(id);
    id[len] = '\n';
    id[len + 1] = '\0';
    Writen(sockfd, id, strlen(id));
    //printf("sent: %s\n", id);
    readline(sockfd, recvline, MAXLINE);
    printf("%s", recvline);

    stdineof = 0;
    peer_exit = 0;
    //clr_scr();
    
    printf("\n");
    printf("\n");
    //clr_scr();
    int highlight = 0;
    
    setup_non_blocking_io();
    //printf("a\n");
    bool is_choose=true;
    print_menu(highlight, choices[0], 3, 1);
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
            //recvlinex=recvline;
            
            //recvline[strcspn(recvline, "\n")] = '\0';
            char *found = strchr(recvline, '_');
            
            found2 = strchr(recvline, 'X');
            if (n == 0) {
                if (stdineof == 1)
                    return;         /* normal termination */
                else {
                    //printf("(End of input from the peer!)\n");
                    peer_exit = 1;
                };
            }
            else if (n > 0) {
                recvline[n] = '\0';
                if (recvline[0] == '5')
                {
                    //printf("game over\n");
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
                    printf("%s\n", recvline);
                }
            }
            memset(recvline,'\0',sizeof(recvline));
            if (found != NULL){
            	is_choose=true;
            	printf("%s\n", recvline);
            	print_menu(highlight, choices[0], 3, 1);
            	found=NULL;
            }
        }
        
        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
            
            int n_choices = 3;
            int start_row = 2;
            if(is_choose){
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
                        printf("(leaving...)\n");
                        stdineof = 1;
                        Shutdown(sockfd, SHUT_WR);      /* send FIN */
                        //restore_original_settings();
                        return 0;
   
                    break;
                case 'e': // Enter键
                    int user_choice = highlight;
                    sprintf(sendline, "%d\n", user_choice+1);
                    is_choose=false;
                    n = strlen(sendline);
                    printf("\n",user_choice+1);
                    Writen(sockfd, sendline, n);
                    
                    break;
                }
                
             }
             else{
             	char ch;
            	read(STDIN_FILENO, &ch, 1);
                switch (ch) {
                case 'q': // 退出

                        printf("(leaving2)\n");
                        stdineof = 1;
                        Shutdown(sockfd, SHUT_WR);      /* send FIN */
                        //restore_original_settings();
                        return 0;
                    
                    break;
                case 'x': // Enter键
               	   if (found2 != NULL){
                        Shutdown(sockfd, SHUT_WR); 
                        return 1;
                        
                   } 
                   break;
                }
            }
            }
           }
         
    
};


int
main(int argc, char** argv)
{
    int					sockfd;
    bool again=1;

    if (argc != 3)
        err_quit("usage: tcpcli <IPaddress> <ID>");
    while(again){
    clr_scr();
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    tcgetattr(STDIN_FILENO, &origt);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    strcpy(id, argv[2]);
    printf("%s\n", ascii_art);
    printf("親愛的%s玩家您好，歡迎進入遊戲\n以下是操作說明\na d 左右移動\nq   退出\ne   送出\n\n", id, ascii_art);
    sleep(3);
    Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    clr_scr();
    again=xchg_data(stdin, sockfd);		/* do it all */
}
    set_blocking_echo();
    printf("aaa");
    exit(0);
}

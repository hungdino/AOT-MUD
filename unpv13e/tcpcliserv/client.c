#include	"unp.h"
#include  <string.h>
#include  <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

char id[MAXLINE];

/* the following two functions use ANSI Escape Sequence */
/* refer to https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797 */

/*
Step 1
處理 argv[1] = IP，argv[2] 當作 ID
Step 2
收 Server 送來的歡迎訊息（e.g. [ID] 您好，歡迎來到 [酷炫的名字]，目前有 n 個玩家等待中，您是第 n+1 位進到遊戲室，其中有 n-m 位想要進入遊戲， m 位想要觀戰。）
Step 3
給玩家兩個選項 (1) 參賽 (2) 觀賽，收使用者選項，檢查選項
Step 4
送使用者選項到 Server
Step 5
收 Server 訊息，可能是 1. 等待更多人加入，持續觀察 Server 有沒有送新的訊息 2. 遊戲開始，分配角色的訊息，開始處理收章節劇情 - 顯示劇情 - 收選項(or None) - 顯示選項 - 送選項給 Server
Step 6
在過程中不斷檢查 Server 送來的訊息有沒有包含特定關鍵字
1. 遊戲開始
2. 遊戲異常終止
3. 遊戲正常終止
*/


void clr_scr() {
	printf("\x1B[2J");
};

void set_scr() {		// set screen to 80 * 25 color mode
	printf("\x1B[=3h");
};

void xchg_data(FILE *fp, int sockfd)
{
    int       maxfdp1, stdineof, peer_exit, n;
    fd_set    rset;
    char      sendline[MAXLINE], recvline[MAXLINE];

	
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
					printf("(End of input from the peer!)");
					peer_exit = 1;
				};
            }
			else if (n > 0) {
				recvline[n] = '\0';
				printf(recvline);
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
				sendline[n] = '\n';
				Writen(sockfd, sendline, n+1);
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

/*
對應 Client
Step 1
Step 2
Server 送歡迎訊息（e.g. [ID] 您好，歡迎來到 [酷炫的名字]，目前有 n 個玩家等待中，您是第 n+1 位進到遊戲室，其中有 n-m 位想要進入遊戲， m 位想要觀戰。）
Step 3
Step 4
收 (1) 參賽 (2) 觀賽
Step 5
Server 送訊息，可能是 1. 等待更多人加入 2. 遊戲開始，分配角色的訊息，開始處理收章節劇情 - 顯示劇情 - 收選項(or None) - 顯示選項 - 送選項給 Server
Step 6（遊戲開始）
在過程中不斷檢查 Server 送來的訊息有沒有包含特定關鍵字
1. 遊戲開始
2. 遊戲終止

進遊戲 (child process)
0. 宣告選項 array
1. 從 struct node 1 取出故事 & 選項 -> 廣播故事
2. 送選項給 client 1
3. 收 client 1 選項
4. 送選項給 client 2
5. 收 client 2 選項
4. 送選項給 client 3
4. 收 client 3 選項
5. call function decideNextNode
6. 進下一個 Node
7. if node == 結局 node
8. 廣播結局，讓 client 切斷連線
*/
#include "story.h"
#include "unp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <time.h>


// Define constants and function prototypes
#define TIMEOUT_SECONDS 10
#define MAX_CLIENTS 10
#define MAX_PLAYERS 3
#define MAX_SPECATORS 7
#define MAX_NODE 15
#define BUFFER_SIZE 1024
#define MAX_ID_LEN 64
#define TO_PLAY 1
#define TO_SPECTATE 2
// 固定選項為 1, 2, 3，初始化（還沒選）為 -1，該選項不能選為 0
#define CHOICE_NOT_AVAILABLE -1
#define CHOICE_1 1
#define CHOICE_2 2
#define CHOICE_3 3
#define CHOICE_INITIAL 0
#define IS_ENDING_NODE 1
#define NOT_ENDING_NODE 0

#define EREN 0
#define MIKASA 1
#define ARMIN 2

void error(const char *msg);
void handle_client_interaction(int sockfd);
void game_process(int* players, int* spectators, int total_players, int total_spectators);
char* get_client_ip(struct sockaddr_in cli_addr);
void send_message(int client_sock, const char* msg);
void sigchld_handler(int s);
void send_welcome_message(int new_client_sock, int players_waiting, int spectators_waiting);
int receive_player_choice(int client_sock);
void send_game_status_message(int client_sock, const char* message);
void broadcast_story(int* client_socks, int num_clients, const char* story);
struct StoryNode* decide_next_node(struct node* current_node, int* choices, int num_choices);
int is_ending_node(int game_node_number);
void broadcast_game_ending(int* client_socks, int num_clients, const char* ending_message);

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    // 建立 Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");

    // Bind Socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = SERV_PORT; // 用 SERV_PORT = 9877
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

    // Socket Listen
    listen(sockfd, MAX_CLIENTS);
    clilen = sizeof(cli_addr);

    // 處理 zombie process
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    // 處理 Client 開始進來的連線
    while (1){
        int players[MAX_PLAYERS];
        int spectators[MAX_SPECATORS];
        int total_clients = 0;
        int total_players = 0;
        int total_spectators = 0;
        // 開始接受 Client 連線
        while(1){
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) {
                perror("ERROR on accept");
                exit(1);
            }
            // 設定 socket timeout 時間
            struct timeval timeout;
            timeout.tv_sec = TIMEOUT_SECONDS;
            timeout.tv_usec = 0;
            if (setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("setsockopt failed\n");
            }
            // 送歡迎訊息
            send_welcome_message(newsockfd, total_players, total_spectators);
            int player_or_spectator = receive_player_choice(newsockfd);
            if (player_or_spectator == TO_PLAY) {
                send_message(newsockfd, "恭喜您，您已編入戰鬥部隊。\n");
                players[total_players] = newsockfd;
                total_players++;
                total_clients++;
            } else if (player_or_spectator == TO_SPECTATE) {
                // 處理 Client 人數過多
                if  (total_clients >= MAX_SPECATORS){
                    send_message(newsockfd, "抱歉，此次戰役編制已滿，請加入下一場戰役。\n");
                    close(newsockfd);
                    continue;
                }
                send_message(newsockfd, "您已被列為支援部隊，請稍候，稍後即可觀戰\n");
                spectators[total_spectators] = newsockfd;
                total_spectators++;
                total_clients++;
            } else{ // player_or_spectator == -1
                // 處理 Client 輸入錯誤
                send_message(newsockfd, "抱歉，司令部無法確認您的志願，請加入下一場戰役。\n");
                close(newsockfd);
                continue;
            }
            // 如果有三個玩家，就開始遊戲
            if (total_players == MAX_PLAYERS) {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("ERROR on fork");
                    exit(1);
                }
                if (pid == 0) {
                    // child process
                    game_process(players, spectators, total_players, total_spectators);
                    exit(0);
                }
            }
        }  
    }
    close(sockfd);
    return 0;
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}



void handle_client_interaction(int sockfd) {
    //receiving choices, sending game updates, etc.
    
}
void game_process(int* players, int* spectators, int total_players, int total_spectators){
    // 固定選項為 1, 2, 3，初始化（還沒選）為 0，該選項不能選為 -1
    int erenChose[MAX_NODE];
    int mikasaChose[MAX_NODE];
    int arminChose[MAX_NODE];
    for (int i = 0; i < MAX_NODE; i++) {
        erenChose[i] = CHOICE_INITIAL;
        mikasaChose[i] = CHOICE_INITIAL;
        arminChose[i] = CHOICE_INITIAL;
    }
    StoryNode* current_node = &Main_node1;
    StoryNode* next_node;

    while (1)
    {
        // 送故事
        broadcast_story(players, total_players, current_node->story);
        broadcast_story(spectators, total_spectators, current_node->story);
        // 送選項
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            int turn = current_node->characterArray[i];
            if (turn == EREN)
            {
                send_options(players[EREN], current_node->Eren);
                int decided_choice = receive_player_choice_1_to_3(players[EREN]);
                erenChose[current_node->nodeSeriesNum] = decided_choice;
            }
            else if (turn == MIKASA)
            {
                send_options(players[MIKASA], current_node->Mikasa);
                int decided_choice = receive_player_choice_1_to_3(players[MIKASA]);
                mikasaChose[current_node->nodeSeriesNum] = decided_choice;
            }
            else if (turn == ARMIN)
            {
                send_options(players[ARMIN], current_node->Armin);
                int decided_choice = receive_player_choice_1_to_3(players[ARMIN]);
                arminChose[current_node->nodeSeriesNum] = decided_choice;
            }
            else
            {
                // 處理錯誤
            }
        if(is_ending_node(current_node->nodeSeriesNum)){
            // 處理結局
            game_ending(players, spectators, total_players, total_spectators, current_node->story);
            break;
        }
        next_node = decideNextNode(current_node, erenChose, mikasaChose, arminChose);
        }
        
    }

}
/*
        fd_set readfds;
        FD_ZERO(&readfds);
        for (int i = 0; i < total_players; i++) {
            FD_SET(players[i], &readfds);
        }
        int maxfd = players[0];
        if (players[1] > maxfd) maxfd = players[1];
        if (players[2] > maxfd) maxfd = players[2];
        maxfd += 1;

        if (select(maxfd, &readfds, NULL, NULL, NULL) < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(players[0], &readfds)) {
            // socket 01 可讀
        }
        if (FD_ISSET(players[1], &readfds)) {
            // socket 02 可讀
        }
        if (FD_ISSET(players[2], &readfds)) {
            // socket 03 可讀
        }
*/



// Function to get client's IP address
char* get_client_ip(struct sockaddr_in cli_addr) {
    return inet_ntoa(cli_addr.sin_addr);
}

void send_message(int client_sock, const char* msg) {
    size_t len = strlen(msg);
    ssize_t total_sent = 0;
    ssize_t sent;

    while (total_sent < len) {
        sent = send(client_sock, msg + total_sent, len - total_sent, 0);
        if (sent < 0) {
            if (errno == EINTR) continue;
            perror("ERROR on send");
            return;
        }
        total_sent += sent;
    }
}

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}


void send_welcome_message(int new_client_sock, int players_waiting, int spectators_waiting){
    char welcome_msg[BUFFER_SIZE];
    int number_of_this_player = players_waiting + spectators_waiting + 1;
    snprintf(welcome_msg, BUFFER_SIZE, "勇敢的士兵您好，歡迎來到瑪利亞之牆奪還戰，您是第 %d 位編列進此戰役的士兵，目前有 %d 位士兵想要親身參與奪還戰， %d 位想要邊支援後勤邊觀戰，請選擇您要 [1] 親身參與奪還戰 [2] 觀戰。\n", number_of_this_player,  players_waiting, spectators_waiting);
    send_message(new_client_sock, welcome_msg);
}

int receive_player_choice_paly_or_spectate(int client_sock){
/*
return: TO_PLAY or TO_SPECTATE or -1
*/
    char buffer[BUFFER_SIZE];
    int n;
    srand(time(NULL));
    if ((n = Readline(client_sock, buffer, BUFFER_SIZE)) <= 0){
        return -1;
    }else{
        int choice = atoi(buffer);
        if (choice == TO_PLAY || choice == TO_SPECTATE){
            return choice;
        }else{
            return -1;
        }
    }
}
int receive_player_choice_1_to_3(int client_sock){
/*
回傳值：選項 1, 2, 3 or random
*/
    char buffer[BUFFER_SIZE];
    int n;
    srand(time(NULL));
    int random = rand() % 3 + 1;
    if ((n = Readline(client_sock, buffer, BUFFER_SIZE)) < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("讀取使用者選項超時，回傳 random。\n");
            return random;
        }else{
            perror("讀取選項錯誤，回傳 random。\n");
            return random;
        }
    }else{
        int choice = atoi(buffer);
        if (choice == 1 || choice == 2 || choice == 3){
            return choice;
        }else{
            return random;
        }
    }
}
void send_options(int client_sock, const char* options){
    char options_msg[BUFFER_SIZE];
    snprintf(options_msg, BUFFER_SIZE, "[選項1]：%s\n[選項2]：%s\n[選項3]：%s\n", options[1], options[2], options[3]);
    send_message(client_sock, options_msg);
}

void broadcast_story(int* client_socks, int num_clients, const char* story);
/*
client_socks: Array of client socket file descriptors.
num_clients: Number of clients to broadcast the story to.
story: The story text to be broadcasted.
*/
int send_option(int client_sock, const char* options){
/*
client_sock: The socket file descriptor for the client.
options: The options to be presented to the client.
*/

}

int is_ending_node(int game_node_number){
/*
game_node: The current game story node to check.
*/
    if(game_node_number >14 && game_node_number != 51 && game_node_number != 52){
        return IS_ENDING_NODE;  
    }
    else{
        return NOT_ENDING_NODE;
    }
}
void game_ending(int* client_socks, int* spectator_socks, int client_num, int spectator_num, const char* ending_message){
    for (int i = 0; i < client_num; i++){
        /* code */
        close(client_socks[i]);
    }
    for (int i = 0; i < spectator_num; i++){
        /* code */
        close(spectator_socks[i]);
    }
}
/*
client_socks: Array of client socket file descriptors.
num_clients: Number of clients to broadcast the ending message to.
ending_message: The ending message of the game.
*/
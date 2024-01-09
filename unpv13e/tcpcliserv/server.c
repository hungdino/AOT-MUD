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

#define TIMEOUT_SECONDS 60
#define MAX_CLIENTS 10
#define MAX_PLAYERS 3
#define MAX_SPECATORS 7
#define MAX_NODE 15
#define BUFFER_SIZE 1024
#define MAX_ID_LEN 64
#define TO_PLAY 1
#define TO_SPECTATE 2
#define IS_ENDING_NODE 1
#define NOT_ENDING_NODE 0

// 固定選項為 1, 2, 3，初始化（還沒選）為 -1，該選項不能選為 0
#define CHOICE_NOT_AVAILABLE -1
#define CHOICE_1 1
#define CHOICE_2 2
#define CHOICE_3 3
#define CHOICE_INITIAL 0

#define EREN 0
#define MIKASA 1
#define ARMIN 2

void error(const char *msg);
void game_process(int* players, int* spectators, int total_players, int total_spectators);
void send_message(int client_sock, const char* msg);
void sigchld_handler(int s);
void send_welcome_message(int new_client_sock, char buffer[BUFFER_SIZE], int players_waiting, int spectators_waiting);
void broadcast_story(int* client_socks, int num_clients, const char* story);
int is_ending_node(int game_node_number);
void broadcast(int* player_sock, int player_num, int*spectator_sock, int spectator_num, const char* msg);
void game_ending(int* client_socks, int* spectator_socks, int client_num, int spectator_num, const char* ending_message);

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
        char buffer[BUFFER_SIZE];
        // 開始接受 Client 連線
        while(1){
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            int n = Readline(newsockfd, buffer, BUFFER_SIZE);
	        if (n > 0) {
    		    buffer[strcspn(buffer, "\n")] = '\0'; // 去掉換行符
	        }
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
            send_welcome_message(newsockfd, buffer  ,total_players, total_spectators);
            int player_or_spectator = receive_player_choice_paly_or_spectate(newsockfd);
            if (player_or_spectator == TO_PLAY) {
                send_message(newsockfd, "恭喜您，您經過訓練軍團的考驗，已獲得資格編入戰鬥部隊，請稍候其他隊員準備完畢即可進入戰場。\n");
                if (total_players == 0){
                    send_message(newsockfd, "接下您，您將作為「艾連」完成瑪利亞之牆奪還戰，為了那自由的天空，獻出你的心臟吧！心臓をささげよ！\n");
                }
                else if (total_players == 1){
                    send_message(newsockfd, "接下您，您將作為「米卡莎」參與瑪利亞之牆奪還戰，為了妳心愛的艾連，獻出妳的心臟吧！心臓をささげよ！\n");
                }
                else if (total_players == 2){
                    send_message(newsockfd, "接下您，您將作為「阿爾敏」帶領瑪利亞之牆奪還戰，為了人類文明的存續，獻出你的心臟吧！心臓をささげよ！\n");
                }
                
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
                send_message(newsockfd, "您已被列為支援部隊，請稍候，稍後即可觀戰。\n");
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
                    printf("以上一局遊戲結束，該局的 child process 正常關閉。\n");
                    exit(0);
                }
            }
            if (total_players == MAX_PLAYERS) {
                break;
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
    StoryNode* current_node;
    StoryNode* next_node = &Main_node1;
    int node_counter = 0;
    int someone_left = 0;
    while (1)
    {
        current_node = next_node;
        node_counter++;
        printf("現在進行了幾個節點： %d\n", node_counter);
        printf("現在的節點是 current_node->nodeSeriesNum = %d\n", current_node->nodeSeriesNum);

        // 判斷是否為結局
        if(is_ending_node(current_node->nodeSeriesNum)){
        // 進入結局
            broadcast(players, total_players, spectators, total_spectators, current_node->story);
            broadcast(players, total_players, spectators, total_spectators, "戰役已結束，請選擇 [1] 重新編隊，或 [2] 使用 Ctrl+C 離開。\n");
            game_ending(players, spectators, total_players, total_spectators, current_node->story);
            break;
        }else if(someone_left == 1){
            broadcast(players, total_players, spectators, total_spectators, "戰役已結束，請選擇 [1] 重新編隊，或 [2] 使用 Ctrl+C 離開。\n");
            game_ending(players, spectators, total_players, total_spectators, current_node->story);
            break;
        }else{
            // 進入新章節，送故事
            broadcast(players, total_players, spectators, total_spectators, current_node->story);
            // 送選項
            for (int i = 0; i < MAX_PLAYERS; i++){
                int turn = current_node->characterArray[i];
                if (turn == EREN)
                {
                    char options[BUFFER_SIZE];
                    snprintf(options, BUFFER_SIZE, "_請艾連做出選擇：\n[1]%s\n[2]%s\n[3]%s\n", current_node->Eren[0], current_node->Eren[1], current_node->Eren[2]);
                    send_message(players[EREN], options);
                    printf("已送出選項給EREN。\n");
                    int decided_choice = receive_player_choice_1_to_3(players[EREN]);
                    if (decided_choice == -1)
                    {
                        printf("廣播EREN中離。\n");
                        broadcast(players, total_players, spectators, total_spectators, "很抱歉，艾連已離我們而去，為了降低傷亡，軍團下令作戰取消，請即刻撤離戰場。\n");
                        someone_left = 1;
                        break;
                    }else{
                        printf("艾連選擇了 %d\n", decided_choice);
                        char choice_message[BUFFER_SIZE];
                        snprintf(choice_message, BUFFER_SIZE, "艾連選擇了：%s\n", current_node->Eren[decided_choice-1]);
                        broadcast(players, total_players, spectators, total_spectators, choice_message);
                    }
                    erenChose[current_node->nodeSeriesNum] = decided_choice;
                }
                else if (turn == MIKASA)
                {
                    char options[BUFFER_SIZE];
                    snprintf(options, BUFFER_SIZE, "_請米卡莎做出選擇：\n[1]%s\n[2]%s\n[3]%s\n", current_node->Mikasa[0], current_node->Mikasa[1], current_node->Mikasa[2]);
                    send_message(players[MIKASA], options);
                    printf("已送出選項給MIKASA。\n");
                    int decided_choice = receive_player_choice_1_to_3(players[MIKASA]);
                    if (decided_choice == -1)
                    {
                        printf("廣播MIKASA中離。\n");
                        broadcast(players, total_players, spectators, total_spectators, "很抱歉，米卡莎已離我們而去，為了降低傷亡，軍團下令作戰取消，請即刻撤離戰場。\n");
                        someone_left = 1;
                        break;
                    }else{
                        printf("米卡莎選擇了 %d\n", decided_choice);
                        char choice_message[BUFFER_SIZE];
                        snprintf(choice_message, BUFFER_SIZE, "米卡莎選擇了：%s\n", current_node->Mikasa[decided_choice-1]);
                        broadcast(players, total_players, spectators, total_spectators, choice_message);
                    }
                    mikasaChose[current_node->nodeSeriesNum] = decided_choice;
                }
                else if (turn == ARMIN)
                {
                    char options[BUFFER_SIZE];
                    snprintf(options, BUFFER_SIZE, "_請阿爾敏做出選擇：\n[1]%s\n[2]%s\n[3]%s\n", current_node->Armin[0], current_node->Armin[1], current_node->Armin[2]);
                    send_message(players[ARMIN], options);
                    printf("已送出選項給ARMIN。\n");
                    int decided_choice = receive_player_choice_1_to_3(players[ARMIN]);
                    if (decided_choice == -1)
                    {
                        printf("廣播ARMIN中離。\n");
                        broadcast(players, total_players, spectators, total_spectators, "很抱歉，阿爾敏已離我們而去，為了降低傷亡，軍團下令作戰取消，請即刻撤離戰場。\n");
                        someone_left = 1;
                        break;
                    }else{
                        printf("阿爾敏選擇了 %d\n", decided_choice);
                        char choice_message[BUFFER_SIZE];
                        snprintf(choice_message, BUFFER_SIZE, "阿爾敏選擇了：%s\n", current_node->Armin[decided_choice-1]);
                        broadcast(players, total_players, spectators, total_spectators, choice_message);
                    }
                    arminChose[current_node->nodeSeriesNum] = decided_choice;
                }
                else
                {
                    printf("這一 turn 有人沒有選項 QQ\n", turn);
                }
            }
        }
        next_node = decide_Next_Node(current_node, node_counter, erenChose, mikasaChose, arminChose);
    }
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


void send_welcome_message(int new_client_sock, char buffer[BUFFER_SIZE] , int players_waiting, int spectators_waiting){
    char welcome_msg[BUFFER_SIZE];
    int number_of_this_play = players_waiting + spectators_waiting + 1;
    snprintf(welcome_msg, BUFFER_SIZE, "勇敢的%s士兵您好，歡迎來到瑪利亞之牆奪還戰，您是第 %d 位編列進此戰役的士兵，目前有 %d 位士兵想要直接參與奪還戰， %d 位想要作為後勤觀戰，請選擇您要 [1] 親身參與奪還戰 [2] 觀戰。\n", buffer, number_of_this_play,  players_waiting, spectators_waiting);
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
        }else {
            perror("讀取選項錯誤，回傳 random。\n");
            return random;
        }
    }else if(n == 0){
        printf("使用者已離開，receive_player_choice_1_to_3 回傳 -1。\n");
        return -1;
    }else{
        int choice = atoi(buffer);
        if (choice == 1 || choice == 2 || choice == 3){
            return choice;
        }else{
            return random;
        }
    }
}

void broadcast(int* player_sock, int player_num, int*spectator_sock, int spectator_num, const char* msg){
    for (int i = 0; i < player_num; i++){
        send_message(player_sock[i], msg);
    }
    for (int i = 0; i < spectator_num; i++){
        send_message(spectator_sock[i], msg);
    }
}


int is_ending_node(int game_node_number){
    if(game_node_number >14 && game_node_number != 51 && game_node_number != 52){
        return IS_ENDING_NODE;  
    }
    else{
        return NOT_ENDING_NODE;
    }
}
void game_ending(int* client_socks, int* spectator_socks, int client_num, int spectator_num, const char* ending_message){
    for (int i = 0; i < client_num; i++){
        close(client_socks[i]);
    }
    for (int i = 0; i < spectator_num; i++){
        close(spectator_socks[i]);
    }
}

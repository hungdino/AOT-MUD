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
#include <pthread.h>


// Define constants and function prototypes
#define MAX_CLIENTS 10
#define MAX_PLAYERS 3
#define MAX_SPECATORS 7
#define MAX_NODE 15
#define BUFFER_SIZE 1024
#define MAX_ID_LEN 64
#define TO_PLAY 1
#define TO_SPECTATE 2
// 固定選項為 1, 2, 3，初始化（還沒選）為 -1，該選項不能選為 0
#define CHOICE_NOT_AVAILABLE 0
#define CHOICE_1 1
#define CHOICE_2 2
#define CHOICE_3 3
#define CHOICE_INITIAL -1

void error(const char *msg);
void handle_client_interaction(int sockfd);
void game_process(int* players, int* spectators, int total_players, int total_spectators);
char* get_client_ip(struct sockaddr_in cli_addr);
void send_message(int client_sock, const char* msg);
void sigchld_handler(int s);
void send_welcome_message(int client_sock, int client_number, int players_waiting, int spectators_waiting);
int receive_player_choice(int client_sock);
void send_game_status_message(int client_sock, const char* message);
void handle_game_story(int client_sock, struct node* game_node);
void broadcast_story(int* client_socks, int num_clients, const char* story);
int send_options_and_receive_choice(int client_sock, const char* options);
struct node* decide_next_node(struct node* current_node, int* choices, int num_choices);
int is_ending_node(struct node* game_node);
void broadcast_game_ending(int* client_socks, int num_clients, const char* ending_message);

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");

    // Bind socket to a port
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = SERV_PORT; // Replace SERV_PORT with your port number
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

    // Listen for incoming connections
    listen(sockfd, MAX_CLIENTS);
    clilen = sizeof(cli_addr);

    int players[MAX_PLAYERS];
    int spectators[MAX_CLIENTS - MAX_PLAYERS];
    int total_clients = 0;
    int total_players = 0;
    int total_spectators = 0;

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }
        send_welcome_message(newsockfd, total_clients, total_players, total_spectators);
        int player_or_spectator = receive_player_choice(newsockfd);
        if (player_or_spectator == TO_PLAY) {
            players[total_players] = newsockfd;
            total_players++;
        } else if (player_or_spectator == TO_SPECTATE && total_clients < MAX_SPECATORS) {
            spectators[total_spectators] = newsockfd;
            total_spectators++;
        } else {
            // Too many clients connected or invalid choice
        }
        total_clients++;
        struct sigaction sa;
        sa.sa_handler = sigchld_handler; // reap all dead processes
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }
        if (total_players == MAX_PLAYERS) {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("ERROR on fork");
                    exit(1);
                }
                if (pid == 0) {
                    // This is the child process
                    // Here you can handle the game with the players and spectators
                    // You can use the arrays 'players' and 'spectators' which contain the socket descriptors
                    // After handling the game, the child process should exit
                    game_process(players, spectators, total_players, total_spectators);
                    exit(0);
                }

                // This is the parent process
                // Reset the players and spectators for the next game
                total_clients = 0;
                total_players = 0;
                total_spectators = 0;
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
    // 固定選項為 1, 2, 3，初始化（還沒選）為 -1，該選項不能選為 0
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
    while (total_players == 3)
    {
        /* code */
        // use select to check if there is any player send message
    }

}



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
            if (errno == EINTR) continue; // Handle interrupted system call
            perror("ERROR on send");
            // Handle error more gracefully, depending on your application logic
            return; // Example: return instead of exit
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


void send_welcome_message(int client_sock, int client_number, int players_waiting, int spectators_waiting){
/*
client_sock: The socket file descriptor for the client.
client_number: The sequential number of the client (n+1 in your example).
total_players: Total number of players currently connected.
players_waiting: Number of players waiting to join the game.
spectators_waiting: Number of spectators waiting to watch the game.
*/
    char welcome_msg[BUFFER_SIZE];
    snprintf(welcome_msg, BUFFER_SIZE, "[ID] 您好，歡迎來到 [酷炫的名字]，目前有 %d 個玩家等待中，您是第 %d 位進到遊戲室，其中有 %d 位想要進入遊戲， %d 位想要觀戰。\n", total_clients, client_number, players, spectators);
    send(client_sock, welcome_msg, strlen(welcome_msg), 0);
}

int receive_player_choice(int client_sock);
/*
client_sock: The socket file descriptor for the client.
*/
void send_game_status_message(int client_sock, const char* message);
/*
client_sock: The socket file descriptor for the client.
message: The status message to be sent (e.g., waiting for more players, game starting, etc.).
*/
void handle_game_story(int client_sock, struct node* game_node);
/*
client_sock: The socket file descriptor for the client.
game_node: A pointer to the current game story node.
*/
void broadcast_story(int* client_socks, int num_clients, const char* story);
/*
client_socks: Array of client socket file descriptors.
num_clients: Number of clients to broadcast the story to.
story: The story text to be broadcasted.
*/
int send_options_and_receive_choice(int client_sock, const char* options){
/*
client_sock: The socket file descriptor for the client.
options: The options to be presented to the client.
*/

}

int is_ending_node(struct node* game_node){
/*
game_node: The current game story node to check.
*/
    if(game_node.nodeSeriesNum >14 && game_node.nodeSeriesNum != 51 && game_node.nodeSeriesNum != 52){
        // 送最後一個劇情
        // 要求 client 給玩家退出或重連
        broadcast_game_ending();
    }
}
void broadcast_game_ending(int* client_socks, int num_clients, const char* ending_message);
/*
client_socks: Array of client socket file descriptors.
num_clients: Number of clients to broadcast the ending message to.
ending_message: The ending message of the game.
*/
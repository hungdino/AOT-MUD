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


#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024
#define MAX_ID_LEN 51

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


void error(const char *msg) {
    perror(msg);
    exit(1);
}

void chatroom(int client1, int client2, char* name1, char* name2) {
    char buffer[BUFFER_SIZE];
    ssize_t nread;
    fd_set readfds;
    int max_sd;
    char message[BUFFER_SIZE + 50]; // Extra space for client name and formatting
    int client1_connected = 1, client2_connected = 1; // Track if clients are connected
    int to_leave_msg_not_sent = 1;
    int first_to_leave = 1;
    while(client1_connected || client2_connected) {
        FD_ZERO(&readfds);
        if (client1_connected) FD_SET(client1, &readfds);
        if (client2_connected) FD_SET(client2, &readfds);
        max_sd = (client1 > client2 ? client1 : client2) + 1;

        // Wait for an activity on one of the sockets
        if (select(max_sd, &readfds, NULL, NULL, NULL) < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        // Handle activity from client1
        if (client1_connected && FD_ISSET(client1, &readfds)) {
            nread = recv(client1, buffer, BUFFER_SIZE, 0);
            if (nread <= 0) {
                client1_connected = 0;
                char disconnect_message[BUFFER_SIZE];
                snprintf(disconnect_message, sizeof(disconnect_message), "(%s left the room. Press Ctrl+D to leave.)\n", name1);
                if (to_leave_msg_not_sent)
                {
                    first_to_leave = 1;
                    send_message(client2, disconnect_message);
                    to_leave_msg_not_sent = 0;
                }
                continue;
            }
            buffer[nread] = '\0'; // Ensure null-termination
            snprintf(message, sizeof(message), "(%s) %s", name1, buffer); // Prepend client1 name
            send(client2, message, strlen(message), 0);
        }

        // Handle activity from client2
        if (client2_connected && FD_ISSET(client2, &readfds)) {
            nread = recv(client2, buffer, BUFFER_SIZE, 0);
            if (nread <= 0) {
                client2_connected = 0;
                char disconnect_message[BUFFER_SIZE];
                snprintf(disconnect_message, sizeof(disconnect_message), "(%s left the room. Press Ctrl+D to leave.)\n", name2);
                if (to_leave_msg_not_sent)
                {
                    first_to_leave = 2;
                    send_message(client1, disconnect_message);
                    to_leave_msg_not_sent = 0;
                }
                continue;
            }
            buffer[nread] = '\0'; // Ensure null-termination
            snprintf(message, sizeof(message), "(%s) %s", name2, buffer); // Prepend client2 name
            send(client1, message, strlen(message), 0);
        }
    }
    
    
    char disconnect_message[BUFFER_SIZE];
    if(first_to_leave == 1){
        snprintf(disconnect_message, sizeof(disconnect_message), "(%s left the room)\n", name2);
        send_message(client1, disconnect_message);
    }else{
        snprintf(disconnect_message, sizeof(disconnect_message), "(%s left the room)\n", name1);
        send_message(client2, disconnect_message);
    }
    // Cleanup
    if (client1_connected) close(client1);
    if (client2_connected) close(client2);
}


void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = SERV_PORT+3;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    printf("IP binded.\n");

    listen(sockfd,5);

    clilen = sizeof(cli_addr);

    // SIGCHLD 
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Global array to hold client sockets
    int client_sockets[MAX_CLIENTS] = {0};
    int client_count = 0;

    char client1_ip[INET_ADDRSTRLEN], client2_ip[INET_ADDRSTRLEN]; // To store client IP addresses
    struct sockaddr_in client_addr[MAX_CLIENTS]; // To store client addresses

    char client_ids[MAX_CLIENTS][MAX_ID_LEN]; // Array to store client IDs

    while(1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
        }
        printf("Connection accepted.\n");

        client_addr[client_count].sin_addr = cli_addr.sin_addr; // Store client's IP

        // Read the client's ID
        ssize_t read_bytes = recv(newsockfd, client_ids[client_count], MAX_ID_LEN - 1, 0);
        if (read_bytes >= 0) {
            if(client_count == 0){
                client_ids[client_count][read_bytes] = '\0'; // Null-terminate the string
                printf("%s is coming.\n", client_ids[client_count]);
                send_message(newsockfd, "You are the 1st user. Wait for the second one!\n");
                printf("msg to first user sent.\n");
                client_sockets[client_count] = newsockfd; // Add the new client to the array
                client_count++;
            }
            else if(client_count == 1){
                client_ids[client_count][read_bytes] = '\0'; // Null-terminate the string
                printf("%s is coming.\n", client_ids[client_count]);
                send_message(newsockfd, "You are the 2nd user\n");
                printf("msg to second user sent.\n");
                client_sockets[client_count] = newsockfd; // Add the new client to the array
                client_count++;
            }
        } else {
            // Handle read error
            printf("Couldn't read the id of the user.\n");
        }

        
        if (client_count == 2) {            
            // Get clients' IP addresses
            strcpy(client1_ip, get_client_ip(client_addr[0]));
            strcpy(client2_ip, get_client_ip(client_addr[1]));

            // Send info about the second client to the first client
            char msg_to_first[1024];
            sprintf(msg_to_first, "The second user is %s from %s\n", client_ids[1], client2_ip);
            send_message(client_sockets[0], msg_to_first);

            // Send info about the first client to the second client
            char msg_to_second[1024];
            sprintf(msg_to_second, "The first user is %s from %s\n", client_ids[0], client1_ip);
            send_message(client_sockets[1], msg_to_second);
            pid = fork();
            if (pid < 0) {
                error("ERROR on fork");
            }
            if (pid == 0) {
                // Child process to handle chatroom
                close(sockfd);

                // Call chatroom with client sockets and names
                printf("Two users entering chatroom\n");
                chatroom(client_sockets[0], client_sockets[1], client_ids[0], client_ids[1]);

                exit(0);
            } else {
                // Parent process closes descriptors and resets the count
                close(client_sockets[0]);
                close(client_sockets[1]);
                client_count = 0;
            }
        }

    }

    close(sockfd);
    return 0; 
}




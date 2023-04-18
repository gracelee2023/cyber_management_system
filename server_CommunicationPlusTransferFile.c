/*
This server file creates two separate connections for 
communication and file transfer. The server receives messages 
from the client on the communication socket and processes 
the file transfer on the file transfer socket.

The communications between server and client involve several preset 
massages indicating the identification and the intent to transfer file.

The client.c and server.c files involve communication between 
the server and client and transfer a file named "cyber.txt" 
from the client to the server.

In client.c, the send_file() function reads data from 
the file -"cyber.txt" and sends it in chunks to the server. 

When the server receives the data, the write_file() function writes 
the data to a new file named "cyber_received.txt". 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define SIZE 1024

void receive_message(int client_fd) {
    char buffer[1000];
    int len = read(client_fd, buffer, sizeof(buffer) - 1);
    buffer[len] = '\0';
    printf("SERVER RECEIVED: %s \n", buffer);
}

void send_message(char *msg, int sock_fd) {
    printf("SERVER SENDING: %s\n", msg);
    write(sock_fd, msg, strlen(msg));
}

void write_file(int sockfd) {
    int n;
    FILE *fp;
    char *filename = "cyber_received.txt";
    char buffer[SIZE];

    fp = fopen(filename, "wb");
    while ((n = recv(sockfd, buffer, SIZE, 0)) > 0) {
        fwrite(buffer, 1, n, fp);
        bzero(buffer, SIZE);
    }
    fclose(fp);
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd, new_comm_sock, new_file_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    // char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e < 0) {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successful.\n");

    if (listen(sockfd, 10) == 0) {
        printf("[+]Listening....\n");
    } else {
        perror("[-]Error in listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_comm_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    printf("Communication connection made: client_fd=%d\n", new_comm_sock);

    receive_message(new_comm_sock);
    send_message("Hello! Here is the server at Boston main campus. I'm listening..", new_comm_sock);

    receive_message(new_comm_sock);
    send_message("Yes please!", new_comm_sock);
    close(new_comm_sock);


    new_file_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    printf("File transfer connection made: client_fd=%d\n", new_file_sock);

    write_file(new_file_sock);
    printf("[+]File data received successfully.\n");

    
    close(new_file_sock);
    close(sockfd);

    return 0;
}
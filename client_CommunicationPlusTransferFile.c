/*
 This client file creates a separate socket for communication "comm_sockfd" 
 and another socket for file transfer "file_sockfd". The communication socket 
 is used for sending and receiving messages between the client and server, 
 while the file transfer socket is solely used for sending the file 
 from the client to the server.

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
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define SIZE 1024

void send_message(char *msg, int sock_fd) {
    printf("SENDING: %s\n", msg);
    write(sock_fd, msg, strlen(msg));
}

void read_response(int sock_fd) {
    char resp[1000];
    int len = read(sock_fd, resp, 999);
    resp[len] = '\0';

    printf("RECEIVED: %s\n", resp);
}


void send_file(FILE *fp, int sockfd) {
    char data[SIZE];
    int n;
    size_t bytes_read;
    while ((bytes_read = fread(data, 1, SIZE, fp)) > 0) {
        n = send(sockfd, data, bytes_read, 0);
        if (n == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int comm_sockfd, file_sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "cyber.txt";

    comm_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (comm_sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(comm_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    send_message("Knock knock, This is NEU SV campus.", comm_sockfd);
    read_response(comm_sockfd);

    send_message("Here is the student info at SV campus. Can I send it now?", comm_sockfd);
    read_response(comm_sockfd);

    // Create another connection for file transfer
    file_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (file_sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]File transfer socket created successfully.\n");

    e = connect(file_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e == -1) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Server for file transfer.\n");

    send_message("Sending file...", comm_sockfd);
    close(comm_sockfd);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }

    send_file(fp, file_sockfd);
    

    printf("[+]File data sent successfully.\n");

    
    close(file_sockfd);

    return 0;
}
/*
  This is the client side implementation of transfering file between server 
  side and client side.
  In these implementations, the client and server can .
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#define SIZE 1024

void send_file(FILE *fp, int sockfd){
  char fileData[SIZE] = {0};
  while(fgets(fileData, SIZE, fp) != NULL) {
    if (send(sockfd, fileData, sizeof(fileData), 0) == 1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    // bzero() places size null bytes in the char data, is used to set all the socket structures with null values
    bzero(fileData, SIZE);
  }
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;

  struct sockaddr_in server_addr;
  
  //Create the socket
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0); /* SOCK_STREAM: TCP */
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET; ///* IPv4 only */
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Try to connect; if I can't, write an error.
  int resp;
  resp = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(resp == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
	printf("[+]Connected to Server.\n");

  // fopen()-opens the file for reading; "r": Opens a file for reading. The file must exist.
  FILE *fp;
  char *filename = "cyber.txt";
  fp = fopen(filename, "r"); 
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  // send file to server
  send_file(fp, sockfd);

  printf("[+]File data sent successfully.\n");

	printf("[+]Closing the connection.\n");

  // clise socket
  close(sockfd);

  return 0;
}

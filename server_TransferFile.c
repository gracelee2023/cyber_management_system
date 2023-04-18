/*
  This is the server implementation of transfering file betwee
  server side and client side.
  In these implementations, the client and server can .
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<unistd.h>
#define SIZE 1024

void write_file(int sockfd){
  FILE *fp;
  char *filename = "cyber_received.txt";
  fp = fopen(filename, "w");
  
  char buffer[SIZE];
  int res;

  while (1) {
    res = recv(sockfd, buffer, SIZE, 0);
    if (res <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main(){
  // Step 1: Get Address
  char *ip = "127.0.0.1"; //"localhost"
  int port = 8080;        // portnumber you pick
  
  // Setting up the sockaddr_in struct
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  // Step 2: Create the socket
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0); /* SOCK_STREAM: TCP */
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET; /* IPv4 only */
  server_addr.sin_port = port; 
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Step 3: Bind the socket
  int resp;
  resp = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(resp < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  // Step 4: Listen
  /* listen(sockfd, 10) is a function call to the listen() function, 
    which is used to make the socket listen for incoming connections.
    "sockfd" - The file descriptor of the socket on which to listen for connections.
    "10" - the server can have up to 10 pending connection requests in its queue.
    listen() returns 0 on success and -1 on failure.
  */
  if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		perror("[-]Error in listening");
    exit(1);
	}

  // Step 5: Accept a connection
  addr_size = sizeof(new_addr);
  int new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  
  // Step 6: write received date from client to "cyber_received.txt"
  write_file(new_sock);
  printf("[+]Data written in the file successfully.\n");
  printf("This file has been received successfully. It is saved by the name cyber_received.txt.");

  // Step 7: Close
  close(new_sock);
 
  return 0;
}

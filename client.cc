#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  
#include <arpa/inet.h>    //close  
#include <unistd.h>   //close  
#define MAX 80 
#define PORT 8888 
#define SA struct sockaddr 

void func(int* sockfds, size_t count) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		for(size_t i = 0; i < count; ++i)
			//write(sockfds[i], buff, sizeof(buff)); 
			write(sockfds[count - i - 1], buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		for(size_t i = 0; i < count; ++i)
		{
			read(sockfds[i], buff, sizeof(buff)); 
			printf("From Server : %s", buff); 
			if ((strncmp(buff, "exit", 4)) == 0) { 
				printf("Client Exit...\n"); 
				break; 
			} 
		}
	} 
} 

int main() 
{ 
	int sockfds[4];
	int count = 0;
	for(int i = 0;i < 4;++i)
	{
		int sockfd; 
		struct sockaddr_in servaddr;

		// socket create and varification 
		sockfd = socket(AF_INET, SOCK_STREAM, 0); 
		if (sockfd == -1) { 
			printf("socket creation failed...\n"); 
			exit(0); 
		} 
		else
			printf("Socket successfully created..\n"); 
		bzero(&servaddr, sizeof(servaddr)); 

		// assign IP, PORT 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		servaddr.sin_port = htons(PORT); 

		// connect the client socket to server socket 
		if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
			printf("connection with the server failed...\n"); 
			exit(0); 
		} 
		else
			printf("connected to the server..\n"); 
		sockfds[i] = sockfd;
		++count;
	}


	// function for chat 
	func(sockfds, count); 

	for(int i = 0; i < 4;++i)
		close(sockfds[i]); 
} 

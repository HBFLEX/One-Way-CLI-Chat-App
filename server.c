#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


void error(const char *msg){
	
	perror(msg);	
	exit(1);
	
	return;
}

void loader(void){
	printf("\t\t\t Connecting to Client...\n");
	sleep(1);
	printf("\t\t\t ...\n");
	sleep(2);
	printf("\t\t\t Connected \n");
	sleep(3);
	system("clear");
	printf("\t\t\t CLI LAN CHAT-APP | Connected\n");

	return;
}

int main(int argc, char* argv[]){

	if (argc < 2){
		fprintf(stderr, "\t\t\t ERROR: Provide the port number for connection \n");
		exit(1);
	}
	
	// initialise variables
	int sockfd, newsockfd, portno, check, n, conn_status;
	char buffer[4096];
	
	struct sockaddr_in server_address, cli_addr;
	socklen_t clilen;
	
	// clear the server structure
	bzero((char *) &server_address, sizeof(server_address));
	
	// create a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("\t\t\t\t\t ERROR: Failed to create socket \n");
	
	// define the server information structure
	portno = atoi(argv[1]);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portno);
		
	
	// bind the socket	
	check = bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address));	
	if (check < 0)
		error("\t\t\t ERROR: Binding Failed \n");
		
	// listen for connections
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	// accept connections
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("\t\t\t ERROR: Failed to accept for incoming connections \n");

	loader();
	
	while(1){
	
		bzero(buffer, 4096);	
		printf("\n");	
		n = read(newsockfd, buffer, 4096);
		if (n < 0)
			error("\t\t\t ERROR: Failed to read incoming data from client \n");
		printf("\t\t\t Client>> %s \n", buffer);
		
		bzero(buffer, 4096);
		printf("Type a message>> ");
		fgets(buffer, 4096, stdin);
		n = write(newsockfd, buffer, strlen(buffer));
		if (n < 0)
			error("\t\t\t ERROR: Failed to send data \n");
		printf("sent...\n");
			
		check = strncmp("exit", buffer, 4);
		if (check == 0)
			break;
	
	}
	
	close(newsockfd);
	close(sockfd);

	return 0;
}

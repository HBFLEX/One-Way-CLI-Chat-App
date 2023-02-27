#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>


void error(const char* msg){
	perror(msg);
	exit(1);
	return;
}

void loader(void){
	
	printf("\t\t\t Making Request to Server...\n");
	sleep(1);
	printf("\t\t\t Connecting to Server...\n");
	sleep(2);
	printf("\t\t\t Connected \n");
	sleep(3);
	system("clear");
	printf("\t\t\t CLI LAN CHAT-APP | Connected\n");

	return;
}

int main(int argc, char *argv[]){
			
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	int sockfd, portno, n, conn_status, check;
	char buffer[4096];
	
	if(argc < 3){
		fprintf(stderr, "ERROR: Provide [Server] Hostname and Port No.\n");
		exit(1);
	}
	
	portno = atoi(argv[2]);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("\t\t\t ERROR: Error Creating Socket \n");
	
	server = gethostbyname(argv[1]);
	
	if(server == NULL){
		fprintf(stderr, "\t\t\t ERROR: No host found by that name \n");
		exit(1);
	}
	
	
	bzero((char *) &serv_addr, sizeof(serv_addr));	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
	conn_status = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	
	if (conn_status < 0)
		error("\t\t\t ERROR: Failed to connect to server \n");
		
	loader();
	
	while(1){
	
		bzero(buffer, 4096);
		printf("Type a message>> ");			
		fgets(buffer, 4096, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		
		if (n < 0)
			error("\t\t\t ERROR: Failed to send data \n");
		printf("sent...\n");	
					
		bzero(buffer, 4096);
		n = read(sockfd, buffer, 4096);
		
		if (n < 0)
			error("\t\t\t ERROR: Failed to receive data \n");
		
		printf("\t\t\t Server>> %s \n", buffer);
		
		check = strncmp("exit", buffer, 4);
		if (check == 0)
			break;
		
	}
	
	close(sockfd);			
	return 0;
}
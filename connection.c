#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>
#define PORT 6667

int sock = 0;

void sendMessage(char* msg){
	if(send(sock, msg, strlen(msg), 0) < 0)
	{
		printf("%s%s\n", "An error has occure when sending the message: ", msg);
	}
}

void setNickname(char* nick){
	char msg[128] = "NICK ";
	strcat(msg, nick);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void setUser(char* nick){
	char msg[128] = "USER ";
	strcat(msg, nick);
	strcat(msg, " 0 * :");
	strcat(msg, nick);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void joinChannel(char* channel){
	char msg[128] = "JOIN ";
	strcat(msg, channel);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void onMessage(char* command, char* args[], int numArgs, char* by){
	if(strcmp(command,"PING") == 0)
	{
		char msg[128] = "PONG ";
		strcat(msg, args[0]);
		strcat(msg, "\r\n");
		sendMessage(msg);
	}
	else
	{
		printf("Unknown command recieved!: ");
		printf("%s ", command);
		for(int index = 0; index < numArgs - 1; index++){
			printf("%s ", args[index]);
		}
		printf("\n");
	}
}

int readLine(char* buffer){
	int length = 0;
	int found = 0;
	char character;
	while(!found){
		int byteIn = recv(sock, &character, 1, 0);
		buffer[length] = character;
		length++;
		if (length >= 2 && buffer[length-2] == '\r' && buffer[length-1] == '\n') {
            		buffer[length-2] = '\0';
			found = 1;
            		return length;
        	}
	}
}

int main(){
	char *ip = "195.154.200.232";
	struct sockaddr_in server_socket;
	struct hostent *host;
	int numbytes;
	char buffer[1024];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		printf("Failed to create the socket! \n");
		return -1;
	}

	host = gethostbyname(ip);
	if(host == NULL)
	{
		printf("Failed to get the host name! \n");
		return -1;
	}

	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(PORT);
	server_socket.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_socket.sin_zero), 8);
	
	if(connect(sock, (struct sockaddr *) &server_socket, sizeof(server_socket)) < 0)
	{
		printf("Failed to connect! \n");
		return -1;
	}
	
	char* userName = "TurkeyBotC";
	setNickname(userName);
	setUser(userName);
	joinChannel("#TheprogrammingTurkey");
	printf("Connected! \n");

	while(1)
	{
		int numbytes = readLine(buffer);
		if(numbytes > 0)
		{
			char* command;
			char** args;
			char* by = "";
			int numArgs = 0;
			char* subStr = strtok (buffer," ");
			if(subStr[0] == ':'){
				by = subStr;
				subStr = strtok (NULL," ");
			}
			command = subStr;
			int index = 0;
			while (subStr != NULL)
			{
				subStr = strtok (NULL, " ");
				args[index] = subStr;
				index++;
			}
			numArgs = index;
			onMessage(command, args, numArgs, by);
		}
	}

	close(sock);
	return 0;
}

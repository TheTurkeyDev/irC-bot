#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define PORT 6667

int connected = 1;
sem_t lock;
int sock = 0;
void (*onMsgPtr)(char*, char*, char*);

void sendMessage(char* msg){
	sem_wait(&lock);
	if(send(sock, msg, strlen(msg), 0) < 0)
	{
		printf("%s%s\n", "An error has occure when sending the message: ", msg);
	}
	sem_post(&lock);
}

void setNickname(char* nick){
	char msg[512] = "NICK ";
	strcat(msg, nick);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void setUser(char* nick){
	char msg[512] = "USER ";
	strcat(msg, nick);
	strcat(msg, " 0 * :");
	strcat(msg, nick);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void joinChannel(char* channel){
	char msg[512] = "JOIN ";
	strcat(msg, channel);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void partChannel(char* channel){
	char msg[512] = "Part ";
	strcat(msg, channel);
	strcat(msg, ":Goodbye\r\n");
	sendMessage(msg);
}

void quitIRC(char* from){
	char msg[512] = "QUIT :Bot instructed to leave channel by ";
	strcat(msg, from);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void sendChat(char* channel, char* toSend){
	char msg[512] = "PRIVMSG ";
	strcat(msg, channel);
	strcat(msg, " :");
	strcat(msg, toSend);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

void onMessage(char* command, char* args[], int numArgs, char* by){
	if(strcmp(command,"PING") == 0){
		char msg[512] = "PONG ";
		strcat(msg, args[0]);
		strcat(msg, "\r\n");
		sendMessage(msg);
	}
	else if(strcmp(command, "PRIVMSG") == 0){
		if(onMsgPtr != NULL){
			char msg[1024];
			int index = 0;
			char channel[128];
			int chanFound = 0;
			for(int i = 0; i < numArgs - 1; i++){
				for(int j = 0; j <= strlen(args[i]) - 1; j++){
					if(args[i][j] != '\0'){
						if(!chanFound){
							if(args[i][j] == ':'){
								chanFound = 1;
								index = -1;
							}
							else{
								channel[index] = args[i][j];
							}
						}
						else{
							msg[index] = args[i][j];
						}
						index++;
					}
				}
				msg[index] = ' ';
				index++;
			}
			msg[index] = '\0';
			char from[128];
			index = 0;
			for(int i = 0; i <= strlen(by) - 1; i++){
				if(by[i] != ':'){
					if(by[i] != '!'){
						from[index] = by[i];
						index++;
					}
					else{
						break;
					}
				}
			}
			from[index] = '\0';
			onMsgPtr(msg, from, channel);
		}
	}
	else if(strcmp(command, "JOIN") == 0){
		printf("%s has joined %s\n", by, args[0]);
	}
	else if(strcmp(command, "PART") == 0){
		printf("%s has left %s\n", by, args[0]);
	}
	else if(strcmp(command, "MODE") == 0){
		printf("Mode changed to %s for %s\n", args[1], args[0]);
	}
	else if(strcmp(command, "NOTICE") == 0){
		char msg[1024];
		int index = 0;
		for(int i = 0; i < numArgs - 1; i++){
			for(int j = 0; j <= strlen(args[i]) - 1; j++){
				if(args[i][j] != '\0' && args[i][j] != ':'){
					msg[index] = args[i][j];
					index++;
				}
			}
			msg[index] = ' ';
			index++;
		}
		msg[index] = '\0';
		printf("NOTICE %s\n", msg);
	}
	else{
		//printf("Unknown command recieved!: ");
		//printf("%s ", command);
		//for(int index = 0; index < numArgs - 1; index++){
		//	printf("%s ", args[index]);
		//}
		//printf("\n");
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

void *inputListener(void *vargp){
	char buffer[1024];
	while(connected){
		int numbytes = readLine(buffer);
		if(numbytes > 0){
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
			while (subStr != NULL){
				subStr = strtok (NULL, " ");
				args[index] = subStr;
				index++;
			}
			numArgs = index;
			onMessage(command, args, numArgs, by);
		}
	}
}

void setMsgHandler(void (*funPtr)(char*, char*, char*)){
	onMsgPtr = funPtr;
}

void initConnect(char* ip){
	sem_init(&lock,0,1);
	struct sockaddr_in server_socket;
	struct hostent *host;
	int numbytes;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		printf("Failed to create the socket! \n");
		return;
	}

	host = gethostbyname(ip);
	if(host == NULL){
		printf("Failed to get the host name! \n");
		return;
	}

	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(PORT);
	server_socket.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_socket.sin_zero), 8);
	
	if(connect(sock, (struct sockaddr *) &server_socket, sizeof(server_socket)) < 0){
		printf("Failed to connect! \n");
		return;
	}
	else{
		printf("Connected! \n");
	}

	pthread_t inputThread; 
    	pthread_create(&inputThread, NULL, inputListener, NULL); 

}

void cleanup(){
 	close(sock);
	connected = 0;
}

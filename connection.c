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
int channelCount = 0;
int mathEquationTotal = 0;

void sendMessage(char* msg){
	sem_wait(&lock);
	if(send(sock, msg, strlen(msg), 0) < 0)
	{
		printf("%s%s\n", "An error has occure when sending the message: ", msg);
	}
	sem_post(&lock);
}

void quitIRC(char* from){
	char msg[512] = "QUIT :Bot instructed to shutdown by ";
	strcat(msg, from);
	strcat(msg, "\r\n");
	sendMessage(msg);
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

	char response[512] = "";
	strcat(response, "IRC Bot options: ");
	sendChat(channel, response);

	response = "";
	strcat(response, "");
	strcat(response, from);
	strcat(response, "");
	sendChat(channel, response);
}

void joinChannel(char* channel){
	char msg[512] = "JOIN ";
	strcat(msg, channel);
	strcat(msg, "\r\n");
	sendMessage(msg);
	channelCount++;
	listOptions(channel);
}

void partChannel(char* channel){
	char msg[512] = "Part ";
	strcat(msg, channel);
	strcat(msg, " :Goodbye\r\n");
	sendMessage(msg);
	channelCount--;
	if(channelCount == 0){
		quitIRC("System");
	}
}

void listOptions(char* channel){
	char response[512] = "";
	strcat(response, "IRC Bot options: ");
	sendChat(channel, response);

	response = "";
	strcat(response, "        Option 1: \"!hi\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "        Option 2: \"!info\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "        Option 3: \"!leave\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "        Option 4: \"!shutdown\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "        Option 5: \"!CalculatorMode\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "To see available options: \"!options\"");
	sendChat(channel, response);
}

void enterCalculatorMode(char* channel){
	char response[512] = "";
	strcat(response, "How the Calculator Works: ");
	sendChat(channel, response);

	response = "";
	strcat(response, "Step 1: select an option from the list of available options.");
	sendChat(channel, response);

	response = "";
	strcat(response, "Step 2: if the option is a mathematical opertion then put a space after the command ");
	sendChat(channel, response);

	response = "";
	strcat(response, "and type the value you wish to conduct the opertion on.");
	sendChat(channel, response);
}

void listCalculatorOptions(char* channel){
	char response[512] = "";
	strcat(response, "Calculator options: ");
	sendChat(channel, response);

	response = "";
	strcat(response, "          Option 1: \"!add\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "          Option 2: \"!subtract\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "          Option 3: \"!multiply\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "          Option 4: \"!divide\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "          Option 5: \"!equal\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "          Option 6: \"!clear\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "          Option 7: \"!exitCalculatorMode\"");
	sendChat(channel, response);

	response = "";
	strcat(response, "To see available options: \"!options\"");
	sendChat(channel, response);
}

void add(char* channel, int toBeAdded){
	mathEquationTotal = mathEquationTotal + toBeAdded;
	char response[512] = "";
	char stringNum[10];
	strcat(response, "Equals: " + itoa(mathEquationTotal,stringNum,10));
	sendChat(channel, response);
}

void subtract(char* channel, int toBeSubtracted){
	mathEquationTotal = mathEquationTotal - toBeSubtracted;
	char response[512] = "";
	char stringNum[10];
	strcat(response, "Equals: " + itoa(mathEquationTotal,stringNum,10));
	sendChat(channel, response);
}

void multiply(char* channel. int toBemultiplied){
	mathEquationTotal = mathEquationTotal * toBemultiplied;
	char response[512] = "";
	char stringNum[10];
	strcat(response, "Equals: " + itoa(mathEquationTotal,stringNum,10));
	sendChat(channel, response);
}

void divide(char* channel. int toBeDivided){
	mathEquationTotal = mathEquationTotal * toBemultiplied;
	char response[512] = "";
	char stringNum[10];
	strcat(response, "Equals: " + itoa(mathEquationTotal,stringNum,10));
	sendChat(channel, response);
}

void equal(char* channel. int toBemultiplied){
	char response[512] = "";
	char stringNum[10];
	strcat(response, "Equals: " + itoa(mathEquationTotal,stringNum,10));
	sendChat(channel, response);
}

void clear(char* channel){
	mathEquationTotal = 0;
	char response[512] = "";
	char stringNum[10];
	strcat(response, "Equals: " + itoa(mathEquationTotal,stringNum,10));
	sendChat(channel, response);
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
			char msg[1024] = "";
			int index = 0;
			char channel[128] = "";
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
		char* username = strtok(by, "!");
		printf("%s has joined %s\n", username + 1, args[0]);
	}
	else if(strcmp(command, "PART") == 0){
		har* username = strtok(by, "!");
		printf("%s has left %s\n", username + 1, args[0]);
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
		/*printf("Unknown command recieved!: ");
		printf("%s ", command);
		for(int index = 0; index < numArgs - 1; index++){
			printf("%s ", args[index]);
		}
		printf("\n");*/
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
	channelCount = 0;
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

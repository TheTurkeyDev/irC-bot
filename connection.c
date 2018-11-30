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

//Sends a message through the currently open socket
void sendMessage(char* msg){
	sem_wait(&lock);
	if(send(sock, msg, strlen(msg), 0) < 0)
	{
		printf("%s%s\n", "An error has occure when sending the message: ", msg);
	}
	sem_post(&lock);
}

//Sends a quit message to the server to disconnect the bot.
void quitIRC(char* from){
	char msg[512] = "QUIT :Bot instructed to shutdown by ";
	strcat(msg, from);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

//Sends a nick message to the server to set the nickname of the bot
void setNickname(char* nick){
	char msg[512] = "NICK ";
	strcat(msg, nick);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

//Sends a user message to the server to initialize the user connection
void setUser(char* nick){
	char msg[512] = "USER ";
	strcat(msg, nick);
	strcat(msg, " 0 * :");
	strcat(msg, nick);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

//Sends a join message to the server for the bot to connect to a specific channel
void joinChannel(char* channel){
	char msg[512] = "JOIN ";
	strcat(msg, channel);
	strcat(msg, "\r\n");
	sendMessage(msg);
	//Increase the amount of channels we are connected to
	channelCount++;
}

//Sends a part message to the server for the bot to leave a specific channel
void partChannel(char* channel){
	char msg[512] = "Part ";
	strcat(msg, channel);
	strcat(msg, " :Goodbye\r\n");
	sendMessage(msg);
	//Decrease the amount of channels we are connected to and if at 0, then disconnect the bot
	channelCount--;
	if(channelCount == 0){
		quitIRC("System");
	}
}

//Sends a chat or "Privmsg" message to the server for the bot to send a cht message in a channel
void sendChat(char* channel, char* toSend){
	char msg[512] = "PRIVMSG ";
	strcat(msg, channel);
	strcat(msg, " :");
	strcat(msg, toSend);
	strcat(msg, "\r\n");
	sendMessage(msg);
}

//When the bot recieves a message from a server
void onMessage(char* command, char* args[], int numArgs, char* by){
	if(strcmp(command,"PING") == 0){
		//Respond to the ping message with a pong to keep the connection alive
		char msg[512] = "PONG ";
		strcat(msg, args[0]);
		strcat(msg, "\r\n");
		sendMessage(msg);
	}
	else if(strcmp(command, "PRIVMSG") == 0){
		if(onMsgPtr != NULL){
			//Parse out the various parts of the privmsg to pass along to the callback the implementing program provided (if one was provided)
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
			//Format the username of the sender to remove uneeded info
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
		char* username = strtok(by, "!");
		printf("%s has left %s\n", username + 1, args[0]);
	}
	else if(strcmp(command, "MODE") == 0){
		printf("Mode changed to %s for %s\n", args[1], args[0]);
	}
	else if(strcmp(command, "NOTICE") == 0){
		char msg[1024];
		int index = 0;
		//Formats the Notice to remove extra charecters that aren't needed to be displayed
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
		//Used to help debug unknown messages
		/*printf("Unknown command recieved!: ");
		printf("%s ", command);
		for(int index = 0; index < numArgs - 1; index++){
			printf("%s ", args[index]);
		}
		printf("\n");*/
	}
}

//Reads input from the socket until "\r\n" is recieved and then returns the recieved message to be parsed
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

//Thread method for receiving messages so that this file doesn't hold up the implementers program and prevent them from performing thier own actions after connecting the bot to the irc server.
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

//Method for the implementing program to set the call-back function for when the bot recieves a privmsg so that the implementer can add their own commands
void setMsgHandler(void (*funPtr)(char*, char*, char*)){
	onMsgPtr = funPtr;
}

//Instatiates the bots connection to the privided ip for an IRC server
void initConnect(char* ip){
	channelCount = 0;
	sem_init(&lock,0,1);
	struct sockaddr_in server_socket;
	struct hostent *host;
	int numbytes;
	//create the socket
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
	
	//Connect the socket to the desired address
	if(connect(sock, (struct sockaddr *) &server_socket, sizeof(server_socket)) < 0){
		printf("Failed to connect! \n");
		return;
	}
	else{
		printf("Connected! \n");
	}

	//Start the input collection thread
	pthread_t inputThread; 
    	pthread_create(&inputThread, NULL, inputListener, NULL); 

}

//Clean up and close our sockets
void cleanup(){
 	close(sock);
	connected = 0;
}

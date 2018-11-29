#include <connection.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pthread.h>

int run = 1;

void onIRCMessage(char* msg, char* from, char* channel) {
	char* command = strtok (msg," ");
	if(strcmp(command, "!hi") == 0){
		char response[512] = "";
		strcat(response, "Hello ");
		strcat(response, from);
		strcat(response, "!");
		sendChat(channel, response);
	}
	else if(strcmp(command, "!info") == 0) {
		char response[512] = "";

		struct utsname unameInfo;
		uname(&unameInfo);

		memset(response, 0, sizeof response);
		strcat(response, "System: ");
		strcat(response, unameInfo.sysname);
		sendChat(channel, response);

		memset(response, 0, sizeof response);
		strcat(response, "Node: ");
		strcat(response, unameInfo.nodename);
		strcat(response, "\n");
		sendChat(channel, response);

		memset(response, 0, sizeof response);
		strcat(response, "Release: ");
		strcat(response, unameInfo.release);
		strcat(response, "\n");
		sendChat(channel, response);

		memset(response, 0, sizeof response);
		strcat(response, "Version: ");
		strcat(response, unameInfo.version);
		strcat(response, "\n");
		sendChat(channel, response);

		memset(response, 0, sizeof response);
		strcat(response, "Machine: ");
		strcat(response, unameInfo.machine);
		strcat(response, "\n");
		sendChat(channel, response);
	}
	else if(strcmp(command, "!leave") == 0) {
		quitIRC(from);
		run = 0;
	}

	printf("Recieved a message from: %s \"%s\" in: %s\n", from, msg, channel);
}

int main(){
	char *ip = "195.154.200.232";
	setMsgHandler(onIRCMessage);
	initConnect(ip);
	char* userName = "TurkeyBotC";
	setUser(userName);
	setNickname(userName);
	joinChannel("#TheprogrammingTurkey");
	
	while(run){
	}
	cleanup();
	return 0;
}



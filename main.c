#include <connection.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pthread.h>

int run = 1;
int calculatorMode = 0;

void onIRCMessage(char* msg, char* from, char* channel) {
	char* command = strtok (msg," ");
	if(calculatorMode){
	 	if(strcmp(command, "!add") == 0){
	 		add(channel, atoi(strtok(msg," ")));
	 	}
		else if(strcmp(command, "!subtract") == 0){
	 		subtract(channel, atoi(strtok(msg," ")));
		}

		else if(strcmp(command, "!multiply") == 0){
	 		multiply(channel, atoi(strtok(msg," ")));
		}

		else if(strcmp(command, "!divide") == 0){
	 		divide(channel, atoi(strtok(msg," ")));
		}

		else if(strcmp(command, "!equal") == 0){
	 		equal(channel);
		}

		else if(strcmp(command, "!clear") == 0){
	 		clear(channel);
		}

		else if(strcmp(command, "!exitCalculatorMode") == 0){
			calculatorMode = 0;
			listOptions(channel);
		}
		else if(strcmp(command, "!options") == 0){
			listCalculatorOptions(channel);
		}
	}
	else{
		if(strcmp(command, "!hi") == 0){
			char response[512] = "";
			strcat(response, "Hello ");
			strcat(response, from);
			strcat(response, "!");
			sendChat(channel, response);
		}
		else if(strcomp(command, "!CalculatorMode") == 0){
			calculatorMode = 1;
			enterCalculatorMode(channel);
			listCalculatorOptions(channel);
		}
		else if(strcomp(command, "!options") == 0){
			listOptions(channel);
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
			partChannel(channel);
		}
		else if(strcmp(command, "!shutdown") == 0) {
			quitIRC(from);
			run = 0;
		}
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
	joinChannel("#turkeydev");
	
	while(run){
	}
	cleanup();
	return 0;
}



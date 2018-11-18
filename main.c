#include <connection.h>
#include <stdio.h>
#include <string.h>

void onIRCMessage(char* msg, char* from) {
	char* command = strtok (msg," ");
	if(strcmp(command, "!hi")){
		char msg[512];
		strcat(msg, "Hello ");
		strcat(msg, from);
		strcat(msg, " !\0");
	}
	printf("Recieved a message from: %s\"%s\"\n", from, msg);
}

int main(){
	char *ip = "195.154.200.232";
	setMsgHandler(onIRCMessage);
	initConnect(ip);
	char* userName = "TurkeyBotC";
	setUser(userName);
	setNickname(userName);
	joinChannel("#TheprogrammingTurkey");
	while(1){
	}
	return 0;
}



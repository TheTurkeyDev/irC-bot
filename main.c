#include <connection.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

void onIRCMessage(char* msg, char* from) {
	char* command = strtok (msg," ");
	if(strcmp(command, "!hi")){
		char msg[512];
		strcat(msg, "Hello ");
		strcat(msg, from);
		strcat(msg, " !\0");
	}

	if(strcmp(command, "!info")) {
		char msg[512];

		struct utsname unameInfo;
		uname(&unameInfo);

		strcat(msg, "System: ");
		strcat(msg, unameInfo.sysname);
		strcat(msg, "\n");

		strcat(msg, "Node: ");
		strcat(msg, unameInfo.nodename);
		strcat(msg, "\n");

		strcat(msg, "Release: ");
		strcat(msg, unameInfo.release);
		strcat(msg, "\n");

		strcat(msg, "Version: ");
		strcat(msg, unameInfo.version);
		strcat(msg, "\n");

		strcat(msg, "Machine: ");
		strcat(msg, unameInfo.machine);
		strcat(msg, "\n");

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



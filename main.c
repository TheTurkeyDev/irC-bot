#include <connection.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int run = 1;
time_t start_time;

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
		partChannel(channel);
	}
	else if(strcmp(command, "!shutdown") == 0) {
		quitIRC(from);
		run = 0;
	}
	else if(strcmp(command, "!uptime") == 0) {
		char response[512] = "";
		time_t s;
		struct timespec spec;
		clock_gettime(CLOCK_REALTIME, &spec);
		s = spec.tv_sec;
		long timesince = s - start_time;
		char days[10];
		sprintf(days, "%ld", timesince / 86400); 
		strcat(response, days);
		strcat(response, " Days, ");
		timesince = timesince % 86400;
		char hours[2];
		sprintf(hours, "%ld", timesince / 3600);
		strcat(response, hours);
		strcat(response, " Hours, ");
		timesince = timesince % 3600;
		char minutes[2];
		sprintf(minutes, "%ld", timesince / 60);
		strcat(response, minutes);
		strcat(response, " Minutes, and ");
		timesince = timesince % 60;
		char seconds[2];
		sprintf(seconds, "%ld", timesince);
		strcat(response, seconds);
		strcat(response, " Seconds!");
		sendChat(channel, response);
	}

	printf("Recieved a message from: %s \"%s\" in: %s\n", from, msg, channel);
}

int main(){
	
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	start_time = spec.tv_sec;
	
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



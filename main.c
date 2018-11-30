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
int calculatorMode = 0;
int mathEquationTotal = 0;

//Lists out the valid commands that the bot accepts
void listOptions(char* channel){
	char response[512] = "";
	strcat(response, "IRC Bot options: ");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "        Option 1: \"!hi\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "        Option 2: \"!info\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "        Option 3: \"!leave\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "        Option 4: \"!shutdown\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "        Option 5: \"!uptime\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "        Option 6: \"!calculatorMode\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "To see available options: \"!options\"");
	sendChat(channel, response);
}

// Sets the bot to be in calculator mode and gives relavent information
void enterCalculatorMode(char* channel){
	char response[512] = "";
	strcat(response, "How the Calculator Works: ");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "Step 1: select an option from the list of available options.");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "Step 2: if the option is a mathematical opertion then put a space after the command ");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "and type the value you wish to conduct the opertion on.");
	sendChat(channel, response);
}

// Lists out the valid commands when the bot is in calculator mode
void listCalculatorOptions(char* channel){
	char response[512] = "";
	strcat(response, "Calculator options: ");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "          Option 1: \"!add\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "          Option 2: \"!subtract\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "          Option 3: \"!multiply\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "          Option 4: \"!divide\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "          Option 5: \"!equal\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "          Option 6: \"!clear\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "          Option 7: \"!exitCalculatorMode\"");
	sendChat(channel, response);

	memset(response, 0, sizeof response);
	strcat(response, "To see available options: \"!options\"");
	sendChat(channel, response);
}

//Performs the add operation
void add(char* channel, int toBeAdded){
	mathEquationTotal = mathEquationTotal + toBeAdded;
	char response[512] = "";
	char stringNum[10];
	sprintf(stringNum, "%d", mathEquationTotal);
	strcat(response, "Equals: ");
	strcat(response, stringNum);
	sendChat(channel, response);
}

//Performs the subtract operation
void subtract(char* channel, int toBeSubtracted){
	mathEquationTotal = mathEquationTotal - toBeSubtracted;
	char response[512] = "";
	char stringNum[10];
	sprintf(stringNum, "%d", mathEquationTotal);
	strcat(response, "Equals: ");
	strcat(response, stringNum);
	sendChat(channel, response);
}

//Performs the multiply operation
void multiply(char* channel, int toBemultiplied){
	mathEquationTotal = mathEquationTotal * toBemultiplied;
	char response[512] = "";
	char stringNum[10];
	sprintf(stringNum, "%d", mathEquationTotal);
	strcat(response, "Equals: ");
	strcat(response, stringNum);
	sendChat(channel, response);
}

//Performs the divide operation
void divide(char* channel, int toBeDivided){
	mathEquationTotal = mathEquationTotal / toBeDivided;
	char response[512] = "";
	char stringNum[10];
	sprintf(stringNum, "%d", mathEquationTotal);
	strcat(response, "Equals: ");
	strcat(response, stringNum);
	sendChat(channel, response);
}

//Outputs what the expression currently equals
void equal(char* channel){
	char response[512] = "";
	char stringNum[10];
	sprintf(stringNum, "%d", mathEquationTotal);
	strcat(response, "Equals: ");
	strcat(response, stringNum);
	sendChat(channel, response);
}

//Clears the calculators current value and resets it to 0
void clear(char* channel){
	mathEquationTotal = 0;
	char response[512] = "";
	char stringNum[10];
	sprintf(stringNum, "%d", mathEquationTotal);
	strcat(response, "Equals: ");
	strcat(response, stringNum);
	sendChat(channel, response);
}

//Calback for when the bot recieves a private message
void onIRCMessage(char* msg, char* from, char* channel) {
	// Grabs the first part of the message. Up to the first space to check for a command.
	char* command = strtok (msg," ");
	if(calculatorMode){
	 	if(strcmp(command, "!add") == 0){
	 		add(channel, atoi(strtok(NULL," ")));
	 	}
		else if(strcmp(command, "!subtract") == 0){
	 		subtract(channel, atoi(strtok(NULL," ")));
		}

		else if(strcmp(command, "!multiply") == 0){
	 		multiply(channel, atoi(strtok(NULL," ")));
		}

		else if(strcmp(command, "!divide") == 0){
	 		divide(channel, atoi(strtok(NULL," ")));
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
		else if(strcmp(command, "!calculatorMode") == 0){
			calculatorMode = 1;
			enterCalculatorMode(channel);
			listCalculatorOptions(channel);
		}
		else if(strcmp(command, "!options") == 0){
			listOptions(channel);
		}
		else if(strcmp(command, "!info") == 0) {
			char response[512] = "";

			// Grabs relavent system information
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
			//Grabs the current time and checks for how long its been since the program started and outputs the time since to the given channel.
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
	}
	printf("Recieved a message from: %s \"%s\" in: %s\n", from, msg, channel);
}

int main(){
	//Load and store the initial start time of the program
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	start_time = spec.tv_sec;

	// Connect to the irc server
	char *ip = "195.154.200.232"; //Freenode's IP
	setMsgHandler(onIRCMessage);
	initConnect(ip);
	char* userName = "TurkeyBotC";	//Bot's username
	setUser(userName);
	setNickname(userName);
	
	joinChannel("#TheprogrammingTurkey");
	joinChannel("#turkeydev");
	
	// Hold execution here so the program doesn't just end
	while(run){
	}
	cleanup();
	return 0;
}



void sendMessage(char* msg);
void setNickname(char* nick);
void setUser(char* nick);
void joinChannel(char* channel);
void sendChat(char* channel, char* toSend);
void setMsgHandler(void (*funPtr)(char*, char*, char*));
void initConnect(char* ip);
void cleanup();


void sendMessage(char* msg);
void setNickname(char* nick);
void setUser(char* nick);
void joinChannel(char* channel);
void setMsgHandler(void (*funPtr)(char*, char*));
void initConnect(char* ip);
void cleanup();


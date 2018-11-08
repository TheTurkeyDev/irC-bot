 #include <stdio.h> 
    #include <stdlib.h> 
    #include <errno.h> 
    #include <string.h> 
    #include <netdb.h> 
    #include <sys/types.h> 
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #include <unistd.h>
#define PORT 6667

int main(){
	char *ip = "chat.freenode.net";
	struct sockaddr_in server_socket;
	struct hostent *host;
	int sock = 0;
	char buffer[1024];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		printf("Failed to create the socket! \n");
		return -1;
	}

	host = gethostbyname(ip);
	if(host == NULL)
	{
		printf("Failed to get the host name! \n");
		return -1;
	}

	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(PORT);
	server_socket.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_socket.sin_zero), 8);
	
	if(connect(sock, (struct sockaddr *) &server_socket, sizeof(struct sockaddr)) == -1)
	{
		printf("Failed to connect! \n");
		return -1;
	}

	printf("Connected!");
}

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
	char *ip = "195.154.200.232";
	struct sockaddr_in server_socket;
	struct hostent *host;
	int sock = 0;
	int numbytes;
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
	
	if(connect(sock, (struct sockaddr *) &server_socket, sizeof(server_socket)) < 0)
	{
		printf("Failed to connect! \n");
		return -1;
	}

	if(send(sock, "NICK TurkeyBotC\r\n", 17, 0) == -1)
	{
		printf("Failed to send nick change!\n");	
	}
	if(send(sock, "USER TurkeyBotC 0 * :TurkeyBotC\r\n", 33, 0) < 0)
	{
		printf("Failed User command!\n");
	}
	if(send(sock, "JOIN #theprogrammingturkey\r\n", 28, 0) == -1)
	{
		printf("Failed to send join command! \n");
	}
	printf("Connected! \n");

	while(1)
	{
		if((numbytes = recv(sock, buffer, 1024, 0)) == -1)
		{
			printf("Recive error! \n");
		}

		buffer[numbytes] = '\0';
		if(numbytes > 0)
		{
			printf("Recived: %s", buffer);
		}
	}

	close(sock);
	return 0;
}

#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

int main(int argc , char *argv[]) 
{
	int socketDesc, clientSock, c, readSize;
	struct sockaddr_in server, client;
	char clientMessage[2000];
	
	//Create socket
    // AF_INET (IPv4 protocol) , AF_INET6 (IPv6 protocol) 
    // SOCK_STREAM: TCP(reliable, connection oriented)
    // SOCK_DGRAM: UDP(unreliable, connectionless)
    // Protocol value for Internet Protocol(IP), which is 0
	socketDesc = socket(AF_INET , SOCK_STREAM , 0);
	if (socketDesc == -1) 
    {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8080);
	
	//Bind the socket to the address and port number specified
	if (bind(socketDesc, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socketDesc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	//accept connection from an incoming client
	clientSock = accept(socketDesc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (clientSock < 0) 
    {
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");
	
    while(1) 
    {
        memset(clientMessage, 0, 2000);
        //Receive a message from client
        if (recv(clientSock, clientMessage, 2000, 0) > 0) 
        {
			char *token = strtok(clientMessage, ",");
			//Select command and name			
			if( strcmp(token,"create") == 0 ){
				char *token2 = strtok(NULL, ",");
				char *tmp = token2;
				char *const argv2[] = {"ecs-egent1", "docker", "build", "--t", tmp, NULL}
			}
			else if( strcmp(token,"stop") == 0 ){
				char *token2 = strtok(NULL, ",");
				char *tmp = token2;
				char *const argv2[] = {"ecs-egent1", "docker", "stop", tmp, NULL}
			}
			else{
				char *token2 = strtok(NULL, ",");
				char *tmp = token2;
				char *const argv2[] = {"ecs-egent1", "docker", "rm", "--force", tmp, NULL}
			}
            printf("received message: %s\n", token);
            //Send the message back to client
            send(clientSock, clientMessage, strlen(clientMessage), 0);
        } 
        else 
        {
            puts("recv failed");
			clientSock = accept(socketDesc, (struct sockaddr *)&client, (socklen_t*)&c);
			if (clientSock < 0) 
            {
				perror("accept failed");
				return 1;
			}
			puts("Connection accepted");
        }
    }
	return 0;
}
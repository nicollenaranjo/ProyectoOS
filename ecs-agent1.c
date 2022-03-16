#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<sys/types.h>
#include <stdlib.h>
// Port 8080

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
            printf("received message: %s\n", clientMessage);

			int validate = 0;
			char *token = strtok(clientMessage, ","); 
			char *tmp;
			printf("token %s\n", token);
			 //Select command and name
			if(strcmp(token, "create") == 0)
			{ 
				printf("creating\n");
				char *token2 = strtok(NULL, ", ");
				tmp = token2;
				printf("tmp %s\n", tmp);
				char *const argv2[] = {"/bin/docker", "create", "-t", "-i", "--name", tmp, "ubuntu:latest", NULL};
				
				int id = fork();
				if (id == 0)
				{
					validate = execv(argv2[0], argv2);
					perror("Return from execv() not expected");
				}
			}
			else if (strcmp(token, "start") == 0)
			{
				char *token2 = strtok(NULL, ", ");
				tmp = token2;
				char *const argv2[] = {"/bin/docker", "start", tmp, NULL};
				
				int id = fork();
				if (id == 0)
				{
					validate = execv(argv2[0], argv2);
					perror("Return from execv() not expected");
				}
			}
			else if (strcmp(token, "stop") == 0)
			{
				char *token2 = strtok(NULL, ", ");
				tmp = token2;
				char *const argv2[] = {"/bin/docker", "stop", tmp, NULL};
				
				int id = fork();
				if (id == 0)
				{
					validate = execv(argv2[0], argv2);
					perror("Return from execv() not expected");
				}
			}
			else if (strcmp(token, "delete") == 0)
			{
				char *token2 = strtok(NULL, ", ");
				tmp = token2;
				char *const argv2[] = {"/bin/docker", "rm", tmp, NULL};
				
				int id = fork();
				if (id == 0)
				{
					validate = execv(argv2[0], argv2);
					perror("Return from execv() not expected");
				}
			}

			printf("validate %s\n", validate);
            //Send the message back to client
			if (validate != -1){
				if(strcmp(token, "create") == 0){
					FILE* output_file = fopen("containers.txt", "a+");
					if(!output_file)
						perror("Error fopen");
						
					fwrite(tmp, 1, strlen(tmp), output_file);
					fwrite(" ", 1, 1, output_file);
					fwrite("8080", 1, 4, output_file);
					fwrite("\n", 1, 1, output_file);
					printf("Done Writing!");
					fclose(output_file);
				}
				else if( strcmp(token, "delete") == 0){
					FILE* output_file = fopen("containers.txt", "r");
				}
				
            	send(clientSock, "accepted", 8, 0);
			}else
				send(clientSock, "failed", 6, 0);
        } 
        else 
        {
            //puts("recv failed");
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
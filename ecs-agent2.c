#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<sys/types.h>
#include <stdlib.h>
// Port 9090

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
	server.sin_port = htons(9090);
	
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
					fwrite("9090", 1, 4, output_file);
					fwrite("\n", 1, 1, output_file);
					printf("Done Writing!");
					fclose(output_file);
				}
				else if( strcmp(token, "delete") == 0){
					FILE* output_file = fopen("containers.txt", "r");
					FILE* tmp_file = fopen("archivo2.txt", "a+");
					if(!output_file || !tmp_file)
						perror("Error fopen");

					char buffer[100];
					char *ref = fgets(buffer, 100, output_file);

					while(ref){
						char *token1 = strtok(buffer, "\n");
						char *tmp2 = token1;
						//printf("%s\n", buffer);
						char *token = strtok(buffer, " "); 

						if( strcmp(token, tmp) != 0 ){
							char *token2 = strtok(NULL, " ");
							fwrite(tmp2, 1, strlen(tmp2), tmp_file);
							fwrite(" ", 1, 1, tmp_file);
							fwrite(token2, 1, 4, tmp_file);
							fwrite("\n", 1, 1, tmp_file);
							//printf("Done Writing!");
						}
						ref = fgets(buffer, 100, output_file);

					}
					fclose(output_file);
					remove("containers.txt");
					fclose(tmp_file);
					FILE* tmp2_file = fopen("archivo2.txt", "r+");
					FILE* new_file = fopen("containers.txt", "a+");
					if(!tmp2_file || !new_file)
						perror("Error fopen");

					char buffer2[100];
					char *ref2 = fgets(buffer2, 100, tmp2_file);
					
					while(ref2){
						char *token1 = strtok(buffer2, "\n");
						char *tmp2 = token1;
						char *token = strtok(buffer2, " "); 
							char *token2 = strtok(NULL, " ");
							fwrite(tmp2, 1, strlen(tmp2), new_file);
							printf("%s\n", token2);
							fwrite(" ", 1, 1, new_file);
							fwrite(token2, 1, 4, new_file);
							fwrite("\n", 1, 1, new_file);
							//printf("Done Writing!");
						
						ref2 = fgets(buffer2, 100, tmp2_file);

					}
					fclose(new_file);
					fclose(tmp2_file);
					remove("archivo2.txt");
				}
            	send(clientSock, "accepted", 8, 0);
			}
			else
				send(clientSock, "failed", 6, 0);
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
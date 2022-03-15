#include <stdio.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write
#define SIZE 1000

int main(int argc , char *argv[]) 
{
    int ordinaryPipe1[2];
    int ordinaryPipe2[2];
    char buffer1[SIZE];
    char buffer2[SIZE];

    pipe(ordinaryPipe1);
    pipe(ordinaryPipe2);

    int id = fork();

    //Child - suscribe-host
    if (id == 0)
    {
        int sock1, sock2;
        int sockUse;
        struct sockaddr_in server;
        char message[1000] , serverReply[2000];

        // Close the unwanted ordinary_pipe read side
        close(ordinaryPipe1[0]);
        // Close write
        close(ordinaryPipe2[1]);

        int readbytes;
        
        //Create socket
        sock1 = socket(AF_INET, SOCK_STREAM, 0);
        if (sock1 == -1) 
        {
            printf("Could not create socket");
        }
        puts("Socket1 created");

        sock2 = socket(AF_INET, SOCK_STREAM, 0);
        if (sock2 == -1) 
        {
            printf("Could not create socket");
        }
        puts("Socket2 created");
        
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons(8080);

        //Connect to remote server
        if (connect(sock1, (struct sockaddr *)&server, sizeof(server)) < 0) 
        {
            perror("connect failed. Error");
            return 1;
        }
        puts("Connected socket1\n");

        server.sin_port = htons(9090);

        //Connect to remote server
        if (connect(sock2, (struct sockaddr *)&server, sizeof(server)) < 0) 
        {
            perror("connect failed. Error");
            return 1;
        }
        puts("Connected socket2\n");
        
        //keep communicating with server
        while(1) 
        {
            // printf("Enter message: ");
            // scanf("%s", message);
            // Read message
            int found = 0;
            while (!found)
            {
                while((readbytes=read(ordinaryPipe2[0], buffer2, SIZE)) > 0)
                {
                    printf("Received string in child: %s ", buffer2);
                    break;
                }
                strcpy(message, buffer2);
                strcpy(buffer2, "");
                printf("pipe done\n");
                found = 1;
            }

            sockUse = sock1;
            
            //Send some data
            printf("sending to agent\n");
            if (strcmp(message, "ignore"))
            {
                if(send(sockUse, message, strlen(message), 0) < 0) 
                {
                    puts("Send failed");
                    // return 1;
                }
                else 
                {
                    puts("send ok");
                }
                
                //Receive a reply from the server
                memset(serverReply, 0, 2000 );
                if(recv(sockUse, serverReply, 2000 , 0) < 0) 
                {
                    puts("recv failed");
                } 
                else 
                {
                    puts("recv ok");
                }
                
                puts("Server reply :");
                puts(serverReply);
                strcpy(buffer1, serverReply);
                printf("pipe send reply: %s\n", buffer1);
                write(ordinaryPipe1[1], buffer1, SIZE);
            }
        }
        
        close(sock1);
        close(sock2);
        close(sockUse);
    }
    //Parent - admin-container
    else
    {
        int socketDesc, clientSock, c, readSize;
        struct sockaddr_in server, client;
        char clientMessage[2000];

        // Parent process closes the unwanted ordinary_pipe write side
        close(ordinaryPipe1[1]);
        // Close read
        close(ordinaryPipe2[0]);

        int readbytes;
        
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
        server.sin_port = htons(7070);
        
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
                // Send message through pipe to child
                strcpy(buffer2, clientMessage);
                printf("pipe send: %s\n", buffer2);
                write(ordinaryPipe2[1], buffer2, SIZE);
                write(ordinaryPipe2[1], "", 0);
                //Send the message back to client
                
                int found = 0;
                while (!found)
                {
                    while((readbytes=read(ordinaryPipe1[0], buffer1, SIZE)) > 0)
                    {
                        printf("Received string in child: %s ", buffer1);
                        break;
                    }
                    strcpy(clientMessage, buffer1);
                    strcpy(buffer1, "");
                    printf("pipe done\n");
                    found = 1;
                }
                send(clientSock, clientMessage, strlen(clientMessage), 0);
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
    }
	return 0;
}
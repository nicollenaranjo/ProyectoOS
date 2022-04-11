#include <stdio.h>
#include <stdlib.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

#define SIZE 1000
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int ordinaryPipe1[2];
int ordinaryPipe2[2];
char buffer1[SIZE];
char buffer2[SIZE];

#define handle_error_en(en, msg) do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0);

int done = 0;
int cleanup_pop_arg = 0;
int cnt = 0;

void cleanup_handler(void *arg) 
{
    printf("Called clean-up handler\n");
    cnt = 0;
}

// thread admin
void* threadAdmin(void *arg) 
{
    pthread_mutex_lock( &mutex1 );
    printf("Admin Started\n");
    //pthread_cleanup_push(cleanup_handler, NULL);
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
        //return 1;
    }
    puts("Connected socket1\n");

    server.sin_port = htons(9090);

    //Connect to remote server
    if (connect(sock2, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("connect failed. Error");
        //return 1;
    }
    puts("Connected socket2\n");
    
    //keep communicating with server
    while(1) 
    {
        // Read message
        int found = 0;
        printf("waiting for pipe\n");
        while (!found)
        {
            //pthread_testcancel(); // Direct access
            while((readbytes = read(ordinaryPipe2[0], buffer2, SIZE)) > 0)
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
            printf("demise\n");
            memset(serverReply, 0, 2000 );
            
            
            int demiser = recv(sockUse, serverReply, 2000 , 0);
            printf("demise2\n");
            if(demiser < 0) 
            {
                puts("recv failed");
            } 
            else 
            {
                puts("recv ok");
            }
            printf("demise 3\n");
            
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
    //pthread_cleanup_pop(1);
    pthread_mutex_unlock( &mutex1 );
}

// thread suscribe
void* threadSuscribe(void *arg)
{
    pthread_mutex_lock( &mutex1 );
    printf("Suscribe started\n");
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
        //return 1;
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
        //return 1;
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
                while((readbytes = read(ordinaryPipe1[0], buffer1, SIZE)) > 0)
                {
                    printf("Received string in child: %s ", buffer1);
                    break;
                }
                strcpy(clientMessage, buffer1);
                strcpy(buffer1, "");
                printf("pipe done\n");
                found = 1;
            }
            char *token = strtok(clientMessage, ","); 
            // List containers 
            if(strcmp(token, "list") == 0){ 
                FILE* read_file = ("containers.txt", "r");
                if(!read_file)
                    perror("Error fopen");
                //int tmp = 0;
                printf("List:\n");
                struct stat sb;
                if (stat("containers.txt" , &sb) == -1) {
                    perror("Error fopen. Not containers created");
                }
                printf("Container | Host\n");
                char* file_contents = malloc(sb.st_size);
                fread(file_contents, sb.st_size, 1, read_file);
                printf("%s\n", file_contents);
                fclose(read_file);
                free(file_contents);
            }
            else
                send(clientSock, clientMessage, strlen(clientMessage), 0);
        } 
        else 
        {
            //puts("recv failed");
            clientSock = accept(socketDesc, (struct sockaddr *)&client, (socklen_t*)&c);
            if (clientSock < 0) 
            {
                perror("accept failed");
                //return 1;
            }
            puts("Connection accepted");
        }
    }
    pthread_mutex_unlock( &mutex1 );
}

int main(int argc , char *argv[]) 
{
    pipe(ordinaryPipe1);
    pipe(ordinaryPipe2);
    
    pthread_t thr, thr2;
    int s, t;

    t = pthread_create(&thr2, NULL, threadSuscribe, NULL);
    if (t != 0)
        handle_error_en(t, "threadSuscribe");

    
    //Child - suscribe-host
    s = pthread_create(&thr, NULL, threadAdmin, NULL);
    if (s != 0)
        handle_error_en(s, "threadAdmin");
        
    sleep(99999999);

    //Parent - admin-container
    /*t = pthread_create(&thr2, NULL, threadSuscribe, NULL);
    if (t != 0)
        handle_error_en(t, "threadSuscribe");*/

    pthread_join(s, NULL);
    pthread_join(t, NULL);
	return 0;
}

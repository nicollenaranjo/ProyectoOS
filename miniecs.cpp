#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <string>

int createContainer(string name) 
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) 
    {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(7070);

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) 
    {
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");
	
    string mesAux = "create" + name;
    strcpy(message, mesAux); 
    
    //Send some data
    if( send(sock , message , strlen(message) , 0) < 0) 
    {
        puts("Send failed");
        return 1;
    } 
    else 
    {
        puts("send ok");
    }
    
    //Receive a reply from the server
    memset ( server_reply, 0, 2000 );
    if( recv(sock , server_reply , 2000 , 0) < 0) 
    {
        puts("recv failed");
        break;
    } 
    else 
    {
        puts("recv ok");
    }
    
    puts("Server reply :");
    puts(server_reply);
	
	close(sock);
	return 0;
}

int startContainer(string name)
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(7070);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    string mesAux = "start" + name;
    strcpy(message, mesAux);

    // Send some data
    if (send(sock, message, strlen(message), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    else
    {
        puts("send ok");
    }

    // Receive a reply from the server
    memset(server_reply, 0, 2000);
    if (recv(sock, server_reply, 2000, 0) < 0)
    {
        puts("recv failed");
        break;
    }
    else
    {
        puts("recv ok");
    }

    puts("Server reply :");
    puts(server_reply);

    close(sock);
    return 0;
}

int stopContainer(string name)
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(7070);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    string mesAux = "stop" + name;
    strcpy(message, mesAux);

    // Send some data
    if (send(sock, message, strlen(message), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    else
    {
        puts("send ok");
    }

    // Receive a reply from the server
    memset(server_reply, 0, 2000);
    if (recv(sock, server_reply, 2000, 0) < 0)
    {
        puts("recv failed");
        break;
    }
    else
    {
        puts("recv ok");
    }

    puts("Server reply :");
    puts(server_reply);

    close(sock);
    return 0;
}

int deleteContainer(string name)
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(7070);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    string mesAux = "delete" + name;
    strcpy(message, mesAux);

    // Send some data
    if (send(sock, message, strlen(message), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    else
    {
        puts("send ok");
    }

    // Receive a reply from the server
    memset(server_reply, 0, 2000);
    if (recv(sock, server_reply, 2000, 0) < 0)
    {
        puts("recv failed");
        break;
    }
    else
    {
        puts("recv ok");
    }

    puts("Server reply :");
    puts(server_reply);

    close(sock);
    return 0;
}

int listContainers(string name)
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(7070);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    string mesAux = "list";
    strcpy(message, mesAux);

    // Send some data
    if (send(sock, message, strlen(message), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    else
    {
        puts("send ok");
    }

    // Receive a reply from the server
    memset(server_reply, 0, 2000);
    if (recv(sock, server_reply, 2000, 0) < 0)
    {
        puts("recv failed");
        break;
    }
    else
    {
        puts("recv ok");
    }

    puts("Server reply :");
    puts(server_reply);

    close(sock);
    return 0;
}
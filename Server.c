/****************************************************************
//
//  NAME:        Dominic Molina
//
//  HOMEWORK:    Program 2
//
//  CLASS:       ICS 451
//
//  INSTRUCTOR:  Ravi Narayan
//
//  DATE:        February 24, 2025
//
//  FILE:        server.c
//
//  DESCRIPTION: Networking using sockets, posix, and C programming
//
//  REFERENCES:  https://www.youtube.com/watch?v=LtXEMwSG5-8
//               https://en.wikipedia.org/wiki/C_POSIX_library
//               https://www.geeksforgeeks.org/c/socket-programming-cc/
//               https://medium.com/@coderx_15963/basic-tcp-ip-networking-in-c-using-posix-9a074d65bb35
//               https://www.cs.dartmouth.edu/~campbell/cs50/socketprogramming.html
//
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int  createSocket(void);
void bindSocket(int serverfd, int port);
void startListening(int serverfd);
void handleClient(int serverfd);

/****************************************************************
//
//  Function name: main
//
//  DESCRIPTION:   Sets up and runs the the program, keeps the 
//                 loop running using handleClient func.
//                 
//
//  Parameters:    argc : number of command line arguments
//                 argv : array of command line argument strings
//                        
//
//  Return values:  0 : program ran without errors
//
****************************************************************/

int main(int argc, char *argv[])
{
    int serverfd;
    int port;

    if (argc != 2)
    {
        printf("only one portnumber\n");
        exit(1);
    }

    port = atoi(argv[1]);
    serverfd = createSocket();
    bindSocket(serverfd, port);
    startListening(serverfd);

    while (1)
    {
        handleClient(serverfd);
    }
    close(serverfd);
    return 0;
}

/****************************************************************
//
//  Function name: createSocket
//
//  DESCRIPTION:   Creates a TCP socket for communication with
//                 clients. Uses SO_REUSEADDR so that the port
//                 can be used by another client rightaway.
//
//  Parameters:    void
//
//  Return values:  serverfd : the file descriptor of the socket
//
****************************************************************/

int createSocket(void)
{
    int serverfd;
    int opt = 1;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
    {
        perror("socket function failed");
        exit(1);
    }
    printf("Socket created\n");

    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    return serverfd;
}

/****************************************************************
//
//  Function name: bindSocket
//
//  DESCRIPTION:   assigns a socket to a specific IP address 
//                 and port number so the server can receive incoming 
//                  client connections
//
//  Parameters:    serverfd : the server socket file descriptor
//                 port : the port number to bind to
//
//  Return values:  none
//
****************************************************************/

void bindSocket(int serverfd, int port)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(port);

    if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind function failed");
        exit(1);
    }
    printf("Bound to port %d.\n", port);
}

/****************************************************************
//
//  Function name: startListening
//
//  DESCRIPTION:   Waits for clients to connect to the network
//
//  Parameters:    serverfd : the server socket file descriptor
//
//  Return values:  none
//
****************************************************************/

void startListening(int serverfd)
{
    if (listen(serverfd, 5) < 0)
    {
        perror("listen failed");
        exit(1);
    }
    printf("waiting for connections\n");
}

/****************************************************************
//
//  Function name: handleClient
//
//  DESCRIPTION:   This handles the connection with client. It sends 
//                 messages to the client.
//
//  Parameters:    serverfd : the server socket file descriptor
//
//  Return values:  none
//
****************************************************************/

void handleClient(int serverfd)
{
    int clientfd;

    struct sockaddr_in client_port;
    socklen_t client_len = sizeof(client_port);


    char *message = "Connection established.\n";
    char *message1 = "If you see this the server is working correctly\n";
    char *message2 = "completed\n";
    printf("Waiting for a client to connect to the server\n");
    clientfd = accept(serverfd, (struct sockaddr *) &client_port, &client_len);

    if (clientfd < 0)
    {
        perror("error connecting to the client");
        return;
    }
    printf("Client connected from %s.\n", inet_ntoa(client_port.sin_addr));

    printf("Sending message\n");
    send(clientfd, message,  strlen(message),  0);
    send(clientfd, message1, strlen(message1), 0);
    send(clientfd, message2, strlen(message2), 0);
    printf("Message sent. \n");
    printf("Closing connection. \n");

    close(clientfd);
}

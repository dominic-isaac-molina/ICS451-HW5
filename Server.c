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
//  DESCRIPTION: A simple HTTP server written in C using POSIX
//               socket libraries. When a client connects, the
//               server sends a greeting message and closes the
//               connection. The server continues to run and
//               accept new connections one at a time.
//
//  REFERENCES:  https://www.youtube.com/watch?v=LtXEMwSG5-8
//               https://en.wikipedia.org/wiki/C_POSIX_library
//               https://www.geeksforgeeks.org/c/socket-programming-cc/
//
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int  createSocket(void);
void bindSocket(int server_fd, int port);
void startListening(int server_fd);
void handleClient(int server_fd);

/****************************************************************
//
//  Function name: main
//
//  DESCRIPTION:   Entry point of the server program. Reads the
//                 port number from the command line argument,
//                 sets up the socket, and runs an infinite loop
//                 to keep accepting new client connections one
//                 at a time.
//
//  Parameters:    argc : number of command line arguments
//                 argv : array of command line argument strings
//                        argv[1] is expected to be the port number
//
//  Return values:  0 : program ran without errors
//
****************************************************************/

int main(int argc, char *argv[])
{
    int server_fd;
    int port;

    if (argc != 2)
    {
        printf("Usage: ./server <port>\n");
        exit(1);
    }

    port = atoi(argv[1]);

    server_fd = createSocket();
    bindSocket(server_fd, port);
    startListening(server_fd);

    while (1)
    {
        handleClient(server_fd);
    }

    close(server_fd);
    return 0;
}

/****************************************************************
//
//  Function name: createSocket
//
//  DESCRIPTION:   Creates a TCP socket using AF_INET and
//                 SOCK_STREAM. Also sets the SO_REUSEADDR option
//                 so the port can be reused immediately after
//                 the server stops without waiting for timeout.
//
//  Parameters:    void
//
//  Return values:  server_fd : the file descriptor of the socket
//
****************************************************************/

int createSocket(void)
{
    int server_fd;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket() failed");
        exit(1);
    }
    printf("Socket created.\n");

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    return server_fd;
}

/****************************************************************
//
//  Function name: bindSocket
//
//  DESCRIPTION:   Binds the server socket to all available
//                 network interfaces on the given port number.
//                 This assigns the server a specific address
//                 and port so clients know where to connect.
//
//  Parameters:    server_fd : the server socket file descriptor
//                 port      : the port number to bind to
//
//  Return values:  none
//
****************************************************************/

void bindSocket(int server_fd, int port)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind() failed");
        exit(1);
    }
    printf("Bound to port %d.\n", port);
}

/****************************************************************
//
//  Function name: startListening
//
//  DESCRIPTION:   Puts the server socket into a listening state
//                 so it can begin accepting incoming connections.
//                 The backlog of 5 means up to 5 connections can
//                 queue up while the server is busy.
//
//  Parameters:    server_fd : the server socket file descriptor
//
//  Return values:  none
//
****************************************************************/

void startListening(int server_fd)
{
    if (listen(server_fd, 5) < 0)
    {
        perror("listen() failed");
        exit(1);
    }
    printf("Listening for connections...\n");
}

/****************************************************************
//
//  Function name: handleClient
//
//  DESCRIPTION:   Accepts a single incoming client connection,
//                 sends the greeting message to the client, then
//                 closes the connection. Prints status messages
//                 at each stage so the flow is easy to follow.
//
//  Parameters:    server_fd : the server socket file descriptor
//
//  Return values:  none
//
****************************************************************/

void handleClient(int server_fd)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char *message = "Connection established. If you see this the server creator is working correctly\n";

    printf("Waiting for a client...\n");

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("accept() failed");
        return;
    }
    printf("Client connected from %s.\n", inet_ntoa(client_addr.sin_addr));

    printf("Sending message to client...\n");
    send(client_fd, message, strlen(message), 0);
    printf("Message sent. Closing connection.\n");

    close(client_fd);
}

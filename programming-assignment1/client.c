/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Yang Wu
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>          /* for printf() and fprintf() */
#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Constants */
#define RCVBUFSIZE 512      /* The receive buffer size */
#define SNDBUFSIZE 512      /* The send buffer size */
#define REPLYLEN 32

/* The main function */
int main(int argc, char *argv[]) {

    int clientSock;                 /* socket descriptor */
    struct sockaddr_in servAddr;    /* server address structure */

    char *accountName;              /* Account Name  */
    char *servIP;                   /* Server IP address  */
    unsigned short servPort;        /* Server Port number */

    char sndBuf[SNDBUFSIZE];        /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];        /* Receive Buffer */

    int balance;                    /* Account balance */

    accountName = argv[1];
    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Get the addditional parameters from the command line */
    servIP = argv[2];               /* Get the server IP address */
    servPort = atoi(argv[3]);       /* Get the server port number */
    char *command = argv[4];
    char *amount = (argc == 6) ? argv[5] : "0";

    /* Create a new TCP socket */
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock < 0) {
        printf("Fail to create socket\n");
        exit(1);
    }

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);
    servAddr.sin_port = htons(servPort);

    /* Establish connecction to the server */
    if (connect(clientSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        printf("Connect failed\n");
        exit(1);
    }

    int sendBytes = 0;
    int recvBytes = 0;

    /* Send account name to the server, and wait for the response */
    sendBytes = send(clientSock, accountName, strlen(accountName), 0);
    recvBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);

    /* Send the command to the server, and wait for the response */
    memset(&rcvBuf, 0, RCVBUFSIZE);
    sendBytes = send(clientSock, command, strlen(command), 0);
    recvBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);

    /* Send the amount to the server, and receive the new balance */
    if (argc == 6) {
        memset(&rcvBuf, 0, RCVBUFSIZE);
        sendBytes = send(clientSock, amount, strlen(amount), 0);
        recvBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
        printf("%s\n", rcvBuf); /* Print the withdraw message */

        /* Ask for the current balance */
        strcpy(sndBuf, "BAL");
        memset(&rcvBuf, 0, RCVBUFSIZE);
        sendBytes = send(clientSock, sndBuf, strlen(sndBuf), 0);
        recvBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
    }

    balance = atoi(rcvBuf);
    close(clientSock);

    /* Receive and print response from the server */
    printf("Account Name: %s, ", accountName);
    printf("and the current Balance: %i\n", balance);

    return 0;
}


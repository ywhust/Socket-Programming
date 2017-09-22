/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Yang Wu
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura
* DESCRIPTION:	Network Server Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>          /* for printf() and fprintf() */
#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <stdlib.h>         /* supports all sorts of functionality */
#include <unistd.h>         /* for close() */
#include <string.h>         /* support any string ops */
#include <time.h>

#define RCVBUFSIZE 512      /* The receive buffer size */
#define SNDBUFSIZE 512      /* The send buffer size */
#define BUFSIZE 40          /* Your name can be as many as 40 chars*/
#define BALANCESIZE 10      /* The balance buffer size */

double updateTimestemp(time_t *timestamp, time_t time);

/* The main function */
int main(int argc, char *argv[]) {

    int serverSock;                         /* Server Socket */
    int clientSock;                         /* Client Socket */
    struct sockaddr_in changeServAddr;      /* Local address */
    struct sockaddr_in changeClntAddr;      /* Client address */
    unsigned short changeServPort;          /* Server port */
    unsigned int clntLen;                   /* Length of address data struct */

    char nameBuf[BUFSIZE];                  /* Buff to store account name from client */
    int  balance;                           /* Place to record account balance result */

    /* Pre-configure each account balance */
    int myCheckingBalance   = 3000;
    int mySavingsBalance    = 5000;
    int myRetirementBalance = 10000;
    int myCollegeBalance    = 1500;

    time_t checkingTimestamp[3] = {0};
    time_t savingsTimestamp[3] = {0};
    time_t retirementTimestamp[3] = {0};
    time_t collegeTimestamp[3] = {0};

    char rcvBuf[RCVBUFSIZE];        /* Buffer to receive message from client */
    char sndBuf[SNDBUFSIZE];        /* Buffer to send message to client */
    char balanceBuf[BALANCESIZE];   /* Buffer to store the current balance */

    /* Read the parameters from command line */
    if (argc > 3) {
        printf("Too many parameters\n");
        exit(1);
    }
    char *changeServIP = "127.0.0.1";   /* Server IP address */
    changeServPort = 2017;
    if (argc == 3) {
        changeServIP = argv[1];
        changeServPort = atoi(argv[2]);
    } else if (argc == 2) {
        changeServPort = atoi(argv[1]);
    }

    /* Create new TCP Socket for incoming requests */
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0) {
        printf("Fail to create socket\n");
        exit(1);
    }

    /* Construct local address structure */
    memset(&changeServAddr, 0, sizeof(changeServAddr));
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(changeServPort);

    /* Bind to local address structure */
    if (bind(serverSock, (struct sockaddr*) &changeServAddr, sizeof(changeServAddr)) < 0) {
        printf("Bind failed\n");
        exit(1);
    }

    /* Listen for incoming connections */
    if (listen(serverSock, 32) < 0) {
        printf("Listen failed\n");
        exit(1);
    }

    /* Loop server forever*/
    while (1) {
        /* Wait for a client to connect */
        printf("-----------------------------------\n");
        printf("Waiting for incoming connections...\n");
        int changeClntAddrLen = sizeof(changeClntAddr);
        clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &changeClntAddrLen);
        if (clientSock < 0) {
            printf("Connection failed.\n");
        } else {
            printf("Connection accepted.\n");
        }

        int sendBytes = 0;
        int recvBytes = 0;

        /* Extract the account name from the packet, store in nameBuf */
        memset(&nameBuf, 0, BUFSIZE);
        recvBytes = recv(clientSock, nameBuf, sizeof(nameBuf), 0);

        /* Look up account balance, store in balance */
        if (strcmp(nameBuf, "myChecking") == 0) {
            balance = myCheckingBalance;
        } else if (strcmp(nameBuf, "mySavings") == 0) {
            balance = mySavingsBalance;
        } else if (strcmp(nameBuf, "myRetirement") == 0) {
            balance = myRetirementBalance;
        } else if (strcmp(nameBuf, "myCollege") == 0) {
            balance = myCollegeBalance;
        }

        // /* Print the current account information */
        // printf("Account Name: %s, and the current Balance: %d\n", nameBuf, balance);

        /* Ask the client for the specific operation */
        memset(&sndBuf, 0, SNDBUFSIZE);
        strcpy(sndBuf, "Balance or Withdraw");
        sendBytes = send(clientSock, sndBuf, strlen(sndBuf), 0);

        /* Receive the specific operation */
        memset(&rcvBuf, 0, RCVBUFSIZE);
        recvBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
        if (strcmp(rcvBuf, "WITHDRAW") == 0) {

            // printf("Withdrawing money...\n");

            /* Ask for the amount */
            memset(&sndBuf, 0, SNDBUFSIZE);
            strcpy(sndBuf, "Please enter the amount");
            sendBytes = send(clientSock, sndBuf, strlen(sndBuf), 0);

            /* Receive the amount from client */
            memset(&rcvBuf, 0, RCVBUFSIZE);
            recvBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);

            /* Timeout */
            time_t currentTime;
            time(&currentTime);
            double diff = 0.0;
            if (strcmp(nameBuf, "myChecking") == 0) {
                diff = updateTimestemp(checkingTimestamp, currentTime);
            } else if (strcmp(nameBuf, "mySavings") == 0) {
                diff = updateTimestemp(savingsTimestamp, currentTime);
            } else if (strcmp(nameBuf, "myRetirement") == 0) {
                diff = updateTimestemp(retirementTimestamp, currentTime);
            } else if (strcmp(nameBuf, "myCollege") == 0) {
                diff = updateTimestemp(collegeTimestamp, currentTime);
            }

            /* Send the withdraw message to client */
            memset(&sndBuf, 0, SNDBUFSIZE);
            int amount = atoi(rcvBuf);
            if (diff <= 60.0) {
                strcpy(sndBuf, "Error: Account Timed Out!");
            } else if (amount <= balance) {
                balance -= amount;
                strcpy(sndBuf, "Withdraw Succeeded!");
            } else {
                strcpy(sndBuf, "Error: Insufficient Funds!");
            }
            sendBytes = send(clientSock, sndBuf, strlen(sndBuf), 0);

            // /* Print the current account information */
            // printf("Account Name: %s, and the current Balance: %d\n", nameBuf, balance);

            /* Update the balance of the specific account */
            if (strcmp(nameBuf, "myChecking") == 0) {
                myCheckingBalance = balance;
            } else if (strcmp(nameBuf, "mySavings") == 0) {
                mySavingsBalance = balance;
            } else if (strcmp(nameBuf, "myRetirement") == 0) {
                myRetirementBalance = balance;
            } else if (strcmp(nameBuf, "myCollege") == 0) {
                myCollegeBalance = balance;
            }

            /* Receive the next command from client */
            memset(&rcvBuf, 0, RCVBUFSIZE);
            recvBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
        }

        if (strcmp(rcvBuf, "BAL") == 0) {
            memset(&balanceBuf, 0, BALANCESIZE);
            sprintf(balanceBuf, "%d", balance);
            sendBytes = send(clientSock, balanceBuf, strlen(balanceBuf), 0);
        }

        close(clientSock);
    }
    return 0;
}

double updateTimestemp(time_t *timestamp, time_t time) {
    if (timestamp[0] == 0) {
        timestamp[0] = time;
        return 100.0;
    } else if (timestamp[1] == 0) {
        timestamp[1] = time;
        return 100.0;
    } else if (timestamp[2] == 0) {
        timestamp[2] = time;
        return 100.0;
    } else {
        time_t temp = timestamp[0];
        timestamp[0] = timestamp[1];
        timestamp[1] = timestamp[2];
        timestamp[2] = time;
        return difftime(time, temp);
    }
}


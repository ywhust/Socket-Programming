/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Your Name Here
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura 
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define REPLYLEN 32

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* server address structure */

    char *accountName;		    /* Account Name  */
    char *servIP;		    /* Server IP address  */
    unsigned short servPort;	    /* Server Port number */
    

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int balance;		    /* Account balance */

    /* Get the Account Name from the command line */
    if (argc != 4) 
    {
	printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort");
	exit(1);
    }
    accountName = argv[1];
    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Get the addditional parameters from the command line */
    /*	    FILL IN	*/

    /* Create a new TCP socket*/
    /*	    FILL IN	*/


    /* Construct the server address structure */
    /*	    FILL IN	 */


    /* Establish connecction to the server */
    /*	    FILL IN	 */

    
    /* Send the string to the server */
    /*	    FILL IN	 */


    /* Receive and print response from the server */
    /*	    FILL IN	 */

    printf("%s\n", accountName);
    printf("Balance is: %i\n", balance);

    return 0;
}


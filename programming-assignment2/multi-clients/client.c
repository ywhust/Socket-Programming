// Copied from https://stackoverflow.com/questions/27668560/multiple-clients-in-c-socket-programming
// Example code: A simple client side code
// Note that, although the server side code provided by https://stackoverflow.com/questions/27668560/multiple-clients-in-c-socket-programming
// also supports multi-clients. However, it is not suitable for this project, unless you use multithreading.

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    int s,k;char buff[50],c[50];

    struct sockaddr_in serverport;

    serverport.sin_family = AF_INET;
    serverport.sin_port = htons(5100);
    serverport.sin_addr.s_addr = inet_addr("0.0.0.0");

    int sockid=socket(PF_INET,SOCK_STREAM,0);   

    int connect_status=connect(sockid,(struct sockaddr *) &serverport,sizeof(serverport));
    if(connect_status<0)
    {
        printf("\nConnection Error\n");
        return(1);
    }
    printf("\nConnected\n");

    gets(buff); // message to be sent to server

     s=send(sockid,buff,50,0);
        k=recv(sockid,c,50,0);

    printf("\nServer : %s \n ",c);
    close(sockid);
}
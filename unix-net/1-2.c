#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

/*
    get time from daytime server
    daytime server port: 13
    open daytime server:
        modify daytime config file: /etc/xinetd.d/daytime-stream
*/

#define MAX 1000

int main(int argc, char **argv)
{
    int ret = 0;
    char data[MAX] = {0};
    if(argc < 2)
    {
        printf("Usage: %s ipaddr\n", argv[0]);
        return 1;
    }

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))  < 0)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_in clientaddr;
    memset((void*)&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(13);
    if((inet_pton(AF_INET, argv[1], &clientaddr.sin_addr)) < 1)
    {
        perror("inet_pton");
        return 1;
    }

    if((connect(sockfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr))) < 0)
    {
        perror("connect");
        return 1;
    }

    int n;
    while((n = read(sockfd, data, MAX)) > 0)
    {
        data[n] = '0';
        if(fputs(data, stdout) == EOF)
        {
            perror("fputs");
            return 1;
        }
    }

    if(n < 0)
    {
        perror("read");
        return 1;
    }

    close(sockfd);

    return 0;
}



#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "collection.h"
#include "files.h"

#define BUFFER_SIZE 1024
#define HTTP_PORT 44444
#define MSG "Connected to client!"
int sendToServer(int sockfd, char *message)
{
    if (send(sockfd, message, strlen(message), 0) == -1)
    {
        printf("ERROR: send failed !\n");
        return -3;
    }
    return 0;
}
int main(int argc, char const *argv[])
{
    int sockfd = 0;
    //int num_bytes = 0;
    char sendBuffer[BUFFER_SIZE], recvBuffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr;

    if (argc == 1)
    {
        printf("No arguments supplied\n");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            printf("argv[%d] = %s\n", i, argv[i]);
        }
    }
    /* Create a socket */
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        fprintf(stderr, "ERROR: Failed to open socket\n");
        return -1;
    }

    // clear the structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // connect to local host
    serv_addr.sin_port = htons(HTTP_PORT);
    //Connect
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("ERROR: connecting failed !\n");
        return -2;
    }

    while (true)
    {
        memset(sendBuffer, 0, BUFFER_SIZE);
        memset(recvBuffer, 0, BUFFER_SIZE);
        //recieve
      	printf(" \nEnter your request:");
		fgets(sendBuffer, BUFFER_SIZE-1, stdin);
		if (write(sockfd, sendBuffer, strlen(sendBuffer)+1) < 0)
		{
			fprintf(stderr, "write to buffer failed\n");
			return -3;
		}
		if (read(sockfd, recvBuffer, BUFFER_SIZE-1) < 0)
		{
			fprintf(stderr, "reading from buffer failed\n");
			return -4;
		}
		
		printf("Received: %s", recvBuffer);
    }

    close(sockfd); // close connections
    return 0;
}

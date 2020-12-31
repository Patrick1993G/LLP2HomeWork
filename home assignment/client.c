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

//describes the values recieved by the server
void describe(char *recvBuffer, char *sendBuffer)
{
    if (strcmp(recvBuffer, "OK\n") == 0)
    {
        printf("%s All values are cleared", recvBuffer);
    }
    else if (strcmp(sendBuffer, "PH\n") == 0 || strcmp(sendBuffer, "ph\n") == 0)
    {
        if (atoi(recvBuffer) < 7 && atoi(recvBuffer) > 4)
        {
            printf("PH level is %s : it is slightly acidic", recvBuffer);
        }
        else if (atoi(recvBuffer) > 0 && atoi(recvBuffer) < 4)
        {
            printf("PH level is %s : it is strongly acidic", recvBuffer);
        }
        else if (atoi(recvBuffer) > 6 && atoi(recvBuffer) < 8)
        {
            printf("PH level is %s : it is neutral", recvBuffer);
        }
        else if (atoi(recvBuffer) > 7 && atoi(recvBuffer) < 9)
        {
            printf("PH level is %s : it is slightly alkaline", recvBuffer);
        }
        else
            printf("PH level is %s : it is strongly alkaline", recvBuffer);
    }
    else if (strcmp(sendBuffer, "MOISTURE\n") == 0 || strcmp(sendBuffer, "moisture\n") == 0)
    {
        if (atoi(recvBuffer) < 4 && atoi(recvBuffer) > 0)
        {
            printf("Moisture level is %s : it is slightly moist", recvBuffer);
        }
        else if (atoi(recvBuffer) > 6 && atoi(recvBuffer) < 11)
        {
            printf("Moisture level is %s : it is wet", recvBuffer);
        }
        else
            printf("Moisture level is %s : it is moderate", recvBuffer);
    }
    else if (strcmp(sendBuffer, "SUNLIGHT\n") == 0 || strcmp(sendBuffer, "sunlight\n") == 0)
    {
        if (atoi(recvBuffer) < 400 && atoi(recvBuffer) > 0)
        {
            printf("Sunlight level is %s : it is dark", recvBuffer);
        }
        else if (atoi(recvBuffer) > 1500 && atoi(recvBuffer) < 2001)
        {
            printf("Sunlight level is %s : it is bright", recvBuffer);
        }
        else
            printf("Sunlight level is %s : it is moderately bright", recvBuffer);
    }
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

    do
    {
        memset(sendBuffer, 0, BUFFER_SIZE);
        memset(recvBuffer, 0, BUFFER_SIZE);
        //recieve
        printf(" \nEnter your request:");

        fgets(sendBuffer, BUFFER_SIZE - 1, stdin);
        if (write(sockfd, sendBuffer, strlen(sendBuffer) + 1) < 0)
        {
            fprintf(stderr, "write to buffer failed\n");
            return -3;
        }
        if (read(sockfd, recvBuffer, BUFFER_SIZE - 1) < 0)
        {
            fprintf(stderr, "reading from buffer failed\n");
            return -4;
        }

        //PASS to Descriptive method to describe the values
        describe(recvBuffer, sendBuffer);
    } while (1);

    close(sockfd); // close connections
    return 0;
}

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "collection.h"
#include "files.h"
#define BUFFER_SIZE 1024
#define NAME_SIZE 100
#define HTTP_PORT 44444
#define MSG "Connected to client!"

void sendTofile(char *sendBuffer, char *recvBuffer, char *fileName)
{
    //get time
    time_t timeStamp = time(NULL);
    char *timeString = ctime(&timeStamp);
    char toFile[BUFFER_SIZE];
    memset(toFile, 0, BUFFER_SIZE);
    sprintf(toFile, "%s : %s --> %s", timeString, sendBuffer, recvBuffer);
    printf("path to file %s\n", fileName);
    writeFile('c', toFile, fileName);
}

//describes the values recieved by the server
void describe(char *recvBuffer, char *sendBuffer, char *filename)
{
    char *path = (char *)malloc(sizeof(char) * 50);
    if (path == NULL)
    {
        fprintf(stderr, "error !\n");
    }
#if defined(DBGCLIENT) || defined(FULL)

    sprintf(path, "./%s", filename);
#endif
    bool reset = false;
    if (strcmp(sendBuffer, "RESET\n") == 0 || strcmp(sendBuffer, "reset\n") == 0)
    {
        printf("Sent reset to server ! waiting for reply...\n");
        if (strcmp(recvBuffer, "OK") == 0)
        { // if server file was deleted
            if (removeFile(path))
            {
                reset = true;
            }
        }
    }
    else if (strcmp(sendBuffer, "PH\n") == 0 || strcmp(sendBuffer, "ph\n") == 0)
    {
        if (atoi(recvBuffer) < 7 && atoi(recvBuffer) >= 4)
        {
            printf("PH level is %s : it is slightly acidic\n", recvBuffer);
        }
        else if (atoi(recvBuffer) > 0 && atoi(recvBuffer) < 4)
        {
            printf("PH level is %s : it is strongly acidic\n", recvBuffer);
        }
        else if (atoi(recvBuffer) > 6 && atoi(recvBuffer) <= 7)
        {
            printf("PH level is %s : it is neutral\n", recvBuffer);
        }
        else if (atoi(recvBuffer) >= 8 && atoi(recvBuffer) <= 11)
        {
            printf("PH level is %s : it is slightly alkaline\n", recvBuffer);
        }
        else if (atoi(recvBuffer) > 11 && atoi(recvBuffer) <= 14)
        {
            printf("PH level is %s : it is strongly alkaline\n", recvBuffer);
        }
        else if (atoi(recvBuffer) <= 0)
        {
            printf("PH level is %s : it is pure acid!\n", recvBuffer);
        }
    }
    else if (strcmp(sendBuffer, "MOISTURE\n") == 0 || strcmp(sendBuffer, "moisture\n") == 0)
    {
        if (atoi(recvBuffer) < 4 && atoi(recvBuffer) > 0)
        {
            printf("Moisture level is %s : it is slightly moist\n", recvBuffer);
        }
        else if (atoi(recvBuffer) > 6 && atoi(recvBuffer) < 11)
        {
            printf("Moisture level is %s : it is wet\n", recvBuffer);
        }
        else
            printf("Moisture level is %s : it is moderate\n", recvBuffer);
    }
    else if (strcmp(sendBuffer, "SUNLIGHT\n") == 0 || strcmp(sendBuffer, "sunlight\n") == 0)
    {
        if (atoi(recvBuffer) < 400 && atoi(recvBuffer) > 0)
        {
            printf("Sunlight level is %s : it is dark\n", recvBuffer);
        }
        else if (atoi(recvBuffer) > 1500 && atoi(recvBuffer) < 2001)
        {
            printf("Sunlight level is %s : it is bright\n", recvBuffer);
        }
        else
            printf("Sunlight level is %s : it is moderately bright", recvBuffer);
    }
    else if (strcmp(sendBuffer, "STATS\n") == 0 || strcmp(sendBuffer, "stats\n") == 0)
    {
        printf("%s Number of calls recorded for each command since last reset\n", recvBuffer);
    }
    else
    {
        printf("Unknown command\n");
    }
    if (reset == false)
    {
//writing to file if FULL is defined
#if defined(DBGCLIENT) || defined(FULL)
        sendTofile(sendBuffer, recvBuffer, path);
#endif
    }
    free(path);
    path = NULL;
}

int main(int argc, char const *argv[])
{
    int sockfd = 0;
    char sendBuffer[BUFFER_SIZE], recvBuffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr;

#if defined(DBGCLIENT) || defined(FULL)
    char fileName[NAME_SIZE];
    memset(fileName, 0, NAME_SIZE);
    if (argc == 1)
    {
        printf("No arguments supplied, Default file name is used !\n");
        //write to file with default file name
        strcpy(fileName, "log.txt");
    }
    else
    { // write to file with the user's file name

        for (int i = 1; i < argc; i++)
        {
            strcpy(fileName, argv[i]);
        }
        printf("Using User's file name : %s", fileName);
    }
#endif
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
    bool on = true;
    do
    {
        
        signal(SIGINT, SIG_IGN);
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
        //exit client 
        if (strcmp(sendBuffer, "EXIT\n") == 0 || strcmp(sendBuffer, "exit\n") == 0){
            on = false;
        }

//PASS to Descriptive method to describe the values
#if defined(DBGCLIENT) || defined(FULL)
        describe(recvBuffer, sendBuffer, fileName);
#else
        describe(recvBuffer, sendBuffer, NULL);
#endif
    } while (on);

    close(sockfd); // close connections
    return 0;
}
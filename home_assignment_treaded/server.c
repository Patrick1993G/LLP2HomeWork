#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "collection.h"
#include "files.h"
#define BUFFER_SIZE 1024
#define HTTP_PORT 44444



void chat(int newsockfd, char buffer[])
{
    //generate a collection
    sensor *recSensData = initialiseCollection();
    char toWrite[BUFFER_SIZE], toReturn[BUFFER_SIZE];
  //add sensor to list
    char path[14]="./server.data";
    readFile(path,recSensData);
    while (1)
    {
        bool reset = false;
        memset(toWrite, 0, BUFFER_SIZE);
        memset(toReturn, 0, BUFFER_SIZE);
        
        sensorData data = generateData();
        //reading
        read(newsockfd, buffer, BUFFER_SIZE - 1);
        printf("Recieved - %s\n", buffer);

        if (strcmp(buffer, "RESET\n") == 0 || strcmp(buffer, "reset\n") == 0)
        {
            
            recSensData = initialiseCollection();

            if (removeFile(path) && !reset)
            {
                sprintf(toReturn, "%s", "OK");
                reset = true;
            }
            else
            {
                sprintf(toReturn, "%s", "Not removed !");
            }
        }
        else if (strcmp(buffer, "PH\n") == 0 || strcmp(buffer, "ph\n") == 0)
        {
            sprintf(toReturn, "%.2f", data.PH);
            recSensData->PH += 1;
        }
        else if (strcmp(buffer, "MOISTURE\n") == 0 || strcmp(buffer, "moisture\n") == 0)
        {
            sprintf(toReturn, "%d", data.MOISTURE);
            recSensData->MOISTURE += 1;
        }
        else if (strcmp(buffer, "SUNLIGHT\n") == 0 || strcmp(buffer, "sunlight\n") == 0)
        {
            sprintf(toReturn, "%d", data.SUNLIGHT);
            recSensData->SUNLIGHT += 1;
        }
        else if (strcmp(buffer, "STATS\n") == 0 || strcmp(buffer, "stats\n") == 0)
        {
            sprintf(toReturn, "PH:%d,MOISTURE:%d,SUNLIGHT:%d", recSensData->PH, recSensData->MOISTURE, recSensData->SUNLIGHT);
        }
        else
        {
            sprintf(toReturn, "UNKNOWN");
        }

        
        //writing to client
        strcpy(toWrite, toReturn);
        printf("Echoing back - %s\n", toWrite);
        write(newsockfd, toWrite, strlen(toWrite) + 1);
        //writing to file
        if (strcmp(toReturn, "UNKNOWN") != 0 && !reset)
        {
            char toFile[BUFFER_SIZE];
            memset(toFile, 0, BUFFER_SIZE);
            sprintf(toFile, "%d %d %d ",
            recSensData->PH, recSensData->MOISTURE, recSensData->SUNLIGHT);
            writeFile('s', toFile,NULL);
        }

        // if message contains exit.
        if (strncmp("exit", buffer, 4) == 0)
        {
            printf("Client Exiting...\n");
            break;
        }
    }
   free(recSensData);
}
int main(int argc, char const *argv[])
{
    int sockfd, newsockfd;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    struct sockaddr_in serv_addr;
    //create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd == -1)
    {
        fprintf(stderr, "ERROR: Failed to open socket\n");
        return 1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(HTTP_PORT);
    // allow to reuse the socket as soon as it stops being active */
    int so_reuse_enabled = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &so_reuse_enabled, sizeof(int)) < 0)
    {
        fprintf(stderr, "Reusing of socket failed\n");
        return -2;
    }
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR: bind() failed\n");
        fprintf(stderr, "Error code: %s\n", strerror(errno));
        return 2;
    }

    if (listen(sockfd, 10) < 0)
    {
        fprintf(stderr, "ERROR: listen() failed\n");
        fprintf(stderr, "Error code: %s\n", strerror(errno));
        return 3;
    }

    for (;;)
    {   printf("Listening for a client...\n");
        newsockfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
        if (newsockfd != -1)
        {
            printf("Accepted connection from client\n");
            chat(newsockfd, buffer);
        }
        else
        {
            char *toWrite = "serverExit";
            fprintf(stderr, "ERROR: connection failed\n");
            fprintf(stderr, "Error code: %s\n", strerror(errno));
            return -5;
        }
        close(newsockfd); //close the temp socket
    }
    close(sockfd);
}
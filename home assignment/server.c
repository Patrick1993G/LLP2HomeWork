#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "collection.h"
#include "files.h"

#define BUFFER_SIZE 1024
#define HTTP_PORT 44444


int generate(int lower, int upper)
{
    srand((unsigned)time(NULL));
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

sensor generateData()
{
    sensor data;
    data.MOISTURE = generate(1, 10);
    data.PH = generate(1, 10);
    data.SUNLIGHT = generate(0, 2000);
    return data;
}
sensor* initialiseCollection(){
    sensor *recSensData = (sensor *)malloc(sizeof(sensor));
    recSensData->MOISTURE =0;
    recSensData->PH =0;
    recSensData->SUNLIGHT=0;
    return recSensData;
}
void updatestats(char* toReturn ,sensor*recSensData){
    sprintf(toReturn,"PH:%d,MOISTURE:%d,SUNLIGHT:%d",recSensData->PH,recSensData->MOISTURE,recSensData->SUNLIGHT);
    strcpy(recSensData->STATS,toReturn);
}
void chat(int newsockfd, char *buffer)
{ 
     //generate a collection
    sensor* recSensData=initialiseCollection();
    bool reset =false;
    char toWrite[BUFFER_SIZE], toReturn[BUFFER_SIZE];
    while (1)
    {  
        memset(toWrite, 0, BUFFER_SIZE);
        memset(toReturn, 0, BUFFER_SIZE);

        sensor data = generateData();
        //reading
        read(newsockfd, buffer, BUFFER_SIZE - 1);
        printf("Recieved - %s\n", buffer);

        if (strcmp(buffer, "RESET\n") == 0 || strcmp(buffer, "reset\n") == 0)
        {
            reset = true;
            recSensData=initialiseCollection();
            strcpy(toReturn, "OK");
            
            #if defined (FULL)
            //delete file
             #endif
        }
        else if (strcmp(buffer, "PH\n") == 0 || strcmp(buffer, "ph\n") == 0)
        {
            sprintf(toReturn, "%d", data.PH);
            recSensData->PH +=1;
            updatestats(toReturn,recSensData);
        }
        else if (strcmp(buffer, "MOISTURE\n") == 0 || strcmp(buffer, "moisture\n") == 0)
        {
            sprintf(toReturn, "%d", data.MOISTURE);
            recSensData->MOISTURE+=1;
            updatestats(toReturn,recSensData);
        }
        else if (strcmp(buffer, "SUNLIGHT\n") == 0 || strcmp(buffer, "sunlight\n") == 0)
        {
            sprintf(toReturn, "%d", data.SUNLIGHT);
            recSensData->SUNLIGHT+=1;
            updatestats(toReturn,recSensData);
        }
        else if (strcmp(buffer, "STATS\n") == 0 || strcmp(buffer, "stats\n") == 0)
        {  
            updatestats(toReturn,recSensData);
            printf("%s",recSensData->STATS);
        }
        else{
            sprintf(toReturn,"UNKNOWN");
        }
        strcpy(toWrite, toReturn);
        printf("Echoing back - %s\n", toWrite);
        //writing
        write(newsockfd, toWrite, strlen(toWrite) + 1);
        // //add sensor to list
        // if(!reset){
        //     add_sensor_at_end(recSensData);
        // }
        
        //writing to file 
        if(strcmp(toReturn,"UNKNOWN") != 0){
            time_t timeStamp = time(NULL);
            char* timeString = ctime(&timeStamp);
            char toFile [BUFFER_SIZE];
            memset(toFile, 0, BUFFER_SIZE);
            sprintf(toFile,"%s : PH: %d Moisture %d Sunlight %d Stats %s",timeString,recSensData->PH,recSensData->MOISTURE,recSensData->SUNLIGHT,recSensData->STATS);
            writeFile('s',toFile);
            printf("\nWriting to file...");
            printf("\nFile updated");
        }
        
        // if message contains exit.
        if (strncmp("exit", buffer, 4) == 0)
        {
            printf("Client Exiting...\n");
            break;
        }
          //free collections
      
    }
      free(recSensData);
}
int main(int argc, char const *argv[])
{
    int sockfd, newsockfd;
    char buffer[BUFFER_SIZE];
    

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
        fprintf(stderr, "Reusing of socket failed");
        return -2;
    }
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR: bind() failed\n");
        fprintf(stderr, "Error code: %s\n", strerror(errno));
        return 2;
    }
    printf("Listening for a client...\n");
    if (listen(sockfd, 10) < 0)
    {
        fprintf(stderr, "ERROR: listen() failed\n");
        fprintf(stderr, "Error code: %s\n", strerror(errno));
        return 3;
    }

    for (;;)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
        if (newsockfd != -1)
        {
            printf("Accepted connection from client\n");
            chat(newsockfd, buffer);
        }
        else
        {
            fprintf(stderr, "ERROR: connection failed\n");
            fprintf(stderr, "Error code: %s\n", strerror(errno));
            return -5;
        }
        close(newsockfd); //close the temp socket
    }
    close(sockfd);
}
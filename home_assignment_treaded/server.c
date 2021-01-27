#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "pthread.h" // for threads
#include "collection.h"

#include "files.h"

#define BUFFER_SIZE 1024
#define HTTP_PORT 44444



void chat(int newsockfd, char buffer[],pthread_t thread_id)
{
    //generate a collection
    sensor *recSensData = initialiseCollection();
    recSensData->ID=thread_id;
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
        int numOfBytes = read(newsockfd, buffer, BUFFER_SIZE - 1);
        	if (numOfBytes == -1) {
			fprintf(stderr, "Error: failed reading from client!\n");
			break;
		}
		else if (numOfBytes == 0) {
			printf("Client exited normally\n");
			break;
		}
        else{
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
            sprintf(toFile, "%d %d %d %d ",
            recSensData->ID,recSensData->PH, recSensData->MOISTURE, recSensData->SUNLIGHT);
            writeFile('s', toFile,NULL);
        }

        // if message contains exit.
        if (strncmp("exit", buffer, 4) == 0)
        {
            printf("Client Exiting...\n");
            break;
        }
    }
    }
       
   free(recSensData);
}
//import #include "pthread.h"
//include -lpthread in make file
//create a method to recv and send after create thread 
//handle shared resources with mutex 
//check ws13
void* clientHandler(void *sock){

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

	int* newsockfd_ptr = (int*)sock;
	int newsockfd = *newsockfd_ptr;
	pthread_t thread_id = pthread_self();

	printf("\nClient %lu using socket %x\n", (unsigned long)thread_id, newsockfd);

	// Start chating
    chat(newsockfd, buffer,(unsigned long)thread_id);
    close(newsockfd);
	return NULL;
}
int main(int argc, char const *argv[])
{
    int sockfd,thread_result;
    struct sockaddr_in serv_addr,client_addr; // add client address 
    socklen_t clientlen = sizeof(client_addr);//get the length of the client address
    //create the thread structure
    pthread_t thread;
    //init the mutex lock for files
    pthread_mutex_init(&file_mutex,NULL);
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
        close(sockfd);
        return 2;
    }
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR: bind() failed\n");
        fprintf(stderr, "Error code: %s\n", strerror(errno));
        close(sockfd);
        return 3;
    }
    printf("Listening for a client...\n");
    if (listen(sockfd, 10) != 0) // max 10 clients
    {
        fprintf(stderr, "ERROR: listen() failed\n");
        fprintf(stderr, "Error code: %s\n", strerror(errno));
        close(sockfd);
        return 4;
    }

    for (;;)
    {   
       
        int*comm = malloc(sizeof(int));
        if(comm == NULL){
            fprintf(stderr,"Error: while locating memory");
            close(sockfd);
        }
        //alter the accept method for multiple client handling 
        *comm = accept(sockfd, (struct sockaddr *)&client_addr, &clientlen);
        if (*comm != -1)
        {
            printf("Accepted connection from client\n");
            thread_result= pthread_create(&thread,NULL,clientHandler,comm);
            if(thread_result != 0){
                fprintf(stderr,"Error while creating thread \n");
                free(comm);
                return 5;
            }
        }
        else
        {
            fprintf(stderr, "ERROR: connection failed\n");
            fprintf(stderr, "Error code: %s\n", strerror(errno));
            free(comm);
            return 6;
        }
    }
    //destroy lock
    pthread_mutex_destroy(&file_mutex);
    close(sockfd);
}
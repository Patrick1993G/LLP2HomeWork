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
    data.MOISTURE = generate(0, 10);
    data.PH = generate(0, 10);
    data.SUNLIGHT = generate(0, 2000);
    return data;
}
char *feedback(char *message)
{
    char *toReturn = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    sensor data = generateData();
    if (strcmp(message, "RESET\n") == 0 || strcmp(message, "reset\n") == 0)
    {
        strcpy(toReturn, "OK");
    }
    else if (strcmp(message, "PH\n") == 0 || strcmp(message, "ph\n") == 0)
    {
        sprintf(toReturn, "%d", data.PH);
    }
    else if (strcmp(message, "MOISTURE\n") == 0 || strcmp(message, "moisture\n") == 0)
    {
        sprintf(toReturn, "%d", data.MOISTURE);
    }
    else if (strcmp(message, "SUNLIGHT\n") == 0 || strcmp(message, "sunlight\n") == 0)
    {
        sprintf(toReturn, "%d", data.SUNLIGHT);
    }
    else if (strcmp(message, "STATS\n") == 0 || strcmp(message, "stats\n") == 0)
    {
    }
    else
    {
        strcpy(toReturn, "Unknown Command");
    }
    // add_sensor_at_end(data);
    // free(data);
    return toReturn;
    free(toReturn);
}
// char* describe(char*message, char*recieved)
// {
//     if(strcmp(recieved,"") ){

//     }
// }
void chat(int newsockfd, char *buffer)
{
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        //reading
        read(newsockfd, buffer, BUFFER_SIZE - 1);
        printf("Recieved - %s\n", buffer);

        char *toWrite = (char *)malloc(sizeof(char) * BUFFER_SIZE);
        memset(toWrite, 0, BUFFER_SIZE);
        strcpy(toWrite, feedback(buffer));
        printf("Echoing back - %s\n", toWrite);
        //writing
        write(newsockfd, toWrite, strlen(toWrite) + 1);
        free(toWrite);
        // if message contains exit.
        if (strncmp("exit", buffer, 4) == 0)
        {
            printf("Server Exiting...\n");
            break;
        }
    }
}
int main(int argc, char const *argv[])
{
    int sockfd, newsockfd;
    char buffer[BUFFER_SIZE];
    //generate a collection
    sensor *RecSensData = (sensor *)malloc(sizeof(sensor));

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

        free(RecSensData);
    }
}
//     if (sockfd != -1)
//     {
//         memset(&serv_addr, 0, sizeof(serv_addr));
//         serv_addr.sin_family = AF_INET;
//         serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//         serv_addr.sin_port = htons(HTTP_PORT);

//         if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != -1)
//         { printf("Listening for a client...\n");
//             if (listen(sockfd, 10) != -1)
//             {
//                 newsockfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
//                 if (newsockfd != -1)
//                 {
//                     printf("Accepted connection from client\n");
//                     chat(newsockfd,buffer);
//                 }else{
//                     fprintf(stderr, "ERROR: connection failed\n");
//                     fprintf(stderr, "Error code: %s\n", strerror(errno));
//                     return -5;
//                 }
//             }
//             else
//             {
//                 fprintf(stderr, "ERROR: listen() failed\n");
//                 fprintf(stderr, "Error code: %s\n", strerror(errno));
//                 return -4;
//             }
//         }
//         else
//         {
//             fprintf(stderr, "ERROR: bind() failed\n");
//             fprintf(stderr, "Error code: %s\n", strerror(errno));
//             return -3;
//         }
//     }
//     else
//     {
//         fprintf(stderr, "ERROR: Failed to open socket\n");
//         return -1;
//     }
//     free(RecSensData);
//         close(sockfd);
// }
//     socklen_t clilen = sizeof(cli_addr);

//     // Create a socket
//     sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//     if (sockfd < 0)
//     {
//         fprintf(stderr, "ERROR: Failed to open socket\n");
//         return -1;
//     }

// 	// allow to reuse the socket as soon as it stops being active */
// 	int so_reuse_enabled = 1;
// 	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, & so_reuse_enabled, sizeof(int)) < 0) {
//     	fprintf(stderr, "Reusing of socket failed");
// 		return -2;
// 	}
//     //Initialize socket structure (sockarrd_in)
// 	memset(&serv_addr, 0, sizeof(serv_addr));
// 	serv_addr.sin_family = AF_INET;
// 	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //local host
// 	serv_addr.sin_port = htons(HTTP_PORT);

//     /* Bind the host address */
// 	if (bind(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0) {
// 		fprintf(stderr, "ERROR: bind() failed\n");
// 		fprintf(stderr, "Error code: %s\n", strerror(errno));
// 		return -3;
// 	}

// 	/* Start listening for the clients (thread blocks) */
// 	if (listen(sockfd, MAX_CONNECTIONS) < 0) {
// 		fprintf(stderr, "ERROR: listen() failed\n");
// 		fprintf(stderr, "Error code: %s\n", strerror(errno));
// 		return -4;
// 	}else printf("Listening...\n");
//   while(1){
//     /* Accept connection from a client */
//     newsockfd = accept(sockfd, (struct sockaddr *) & cli_addr, &clilen);
//     if (newsockfd < 0) {
//         fprintf(stderr, "ERROR: accept() failed\n");
//         return -5;
//     }else printf("Accepted connection from client\n");

//      /* Clear buffer and start communicating */
//     memset(buffer, 0, BUFFER_SIZE);
//     num_bytes = recv(newsockfd, buffer, BUFFER_SIZE-1, 0);
//     if (num_bytes < 0) {
//         fprintf(stderr, "ERROR: recv() failed\n");
//         return -6;
//     }
//     printf("Recieved: %s\n", buffer);
//     char* RESPONSE = "connected to Server!\n";
//     /* Write a response to the client */
//     printf("Sending: %s\n", RESPONSE);
//     num_bytes = send(newsockfd, RESPONSE, strlen(RESPONSE), 0);
//     if (num_bytes < 0) {
//         fprintf(stderr, "ERROR: send() failed\n");
//         return -7;
//     }
//     close(newsockfd);//close the temp socket
//     }
//     close(sockfd);
//     return 0;
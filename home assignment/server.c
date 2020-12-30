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
char* feedback(char* message){
    char* toReturn = (char*)malloc(sizeof(char)*BUFFER_SIZE);

    if(strcmp(message,"RESET") == 0){

    }else if(strcmp(message,"PH") == 0 || strcmp(message,"ph") == 0){
         sprintf (toReturn, "%d", generate(10,0,10));
    }else if(strcmp(message,"MOISTURE") == 0){

    }else if(strcmp(message,"SUNLIGHT") == 0){

    }else if(strcmp(message,"STATS") == 0){

    }else{
        strcpy(toReturn,"Unknown Command");
    }
    return toReturn;
}
int main(int argc, char const *argv[])
{
    int sockfd, newsockfd;
    char buffer[BUFFER_SIZE];
    
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //generate a collection
    sensor *RecSensData = (sensor*)malloc(sizeof(sensor)*10);
    
    if (sockfd != -1)
    {
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        serv_addr.sin_port = htons(HTTP_PORT);

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != -1)
        {
            if (listen(sockfd, 10) != -1)
            {
                newsockfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
                if (newsockfd != -1)
                {
                    printf("Accepted connection from client\n");
                    while (1)
                    {
                        memset(buffer, 0, BUFFER_SIZE);
                        read(newsockfd, buffer, BUFFER_SIZE - 1);
                        printf("Recieved - %s\n", buffer);
                        char*toWrite=(char*)malloc(sizeof(char)*BUFFER_SIZE);
                        strcpy(toWrite,feedback(buffer));
                        printf("Echoing back - %s\n", toWrite);
                        write(newsockfd, toWrite, strlen(toWrite) + 1);
                        free(toWrite);
                    }
                }else{
                    fprintf(stderr, "ERROR: connection failed\n");
                    fprintf(stderr, "Error code: %s\n", strerror(errno));
                    return -5;
                }
            }
            else
            {
                fprintf(stderr, "ERROR: listen() failed\n");
                fprintf(stderr, "Error code: %s\n", strerror(errno));
                return -4;
            }
        }
        else
        {
            fprintf(stderr, "ERROR: bind() failed\n");
            fprintf(stderr, "Error code: %s\n", strerror(errno));
            return -3;
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Failed to open socket\n");
        return -1;
    }
    free(RecSensData);
}
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
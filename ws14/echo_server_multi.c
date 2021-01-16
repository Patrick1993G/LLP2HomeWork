#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>		/* strlen, memset */
#include <ctype.h>		/* toupper */
#include <stdlib.h>
#include "echo.h"

void* handle_client(void* socket) {
	char str[MAX_STR];
	int* comm_fd = (int*)socket;
	while(1) {
		memset(str, 0, MAX_STR);
		int num_bytes = read(*comm_fd, str, MAX_STR-1);
		if (num_bytes == -1) {
			fprintf(stderr, "Error: Reading from client failed, killing thread...\n");
			break;
		}
		else if (num_bytes == 0) {
			printf("Client disconnected normally, killing thread...\n");
			break;
		}
		else {
			int i = 0;
			while(str[i]) {
				str[i] = toupper(str[i]);
				i++;
			}

			if (write(*comm_fd, str, strlen(str)+1) < 0) {
				fprintf(stderr, "Error: failed to write to client, killing thread...\n");
				break;
			}
			printf("Echoed: %s", str);
		}
	}

	free(socket);
	return NULL;
}

int main(int argc,char **argv) {
	int listen_fd;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clilen = sizeof(cliaddr);
	pthread_t mythread;

	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_fd == -1) {
		fprintf(stderr, "ERROR: socket()\n");
		return -1;
	}

	int enable_reuse = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(int)) < 0) {
		fprintf(stderr, "ERROR: setsockopt(SO_REUSEADDR) failed\n");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servaddr.sin_port = htons(LISTEN_PORT);

	if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
		fprintf(stderr, "ERROR: bind()\n");
		goto cleanup_server;
	}

	if (listen(listen_fd, 10) == -1) {
		fprintf(stderr, "ERROR: listen()\n");
		goto cleanup_server;
	}

	while (1) {
		printf("Accepting new connections...\n");
		int* comm_fd = malloc(sizeof(int));
		if (comm_fd == NULL)  {
			fprintf(stderr, "ERROR: malloc() failed\n");
			goto cleanup_server;
		}
		*comm_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &clilen);
		if (*comm_fd == -1) {
			fprintf(stderr, "ERROR: accept()\n");
			free(comm_fd);
			goto cleanup_server;
		}
		printf(" -> New Client\n");

		if (pthread_create(&mythread, NULL, handle_client, comm_fd) != 0) {
			fprintf(stderr, "ERROR: pthread_create()\n");
			free(comm_fd);
			goto cleanup_server;
		}
	}

cleanup_server:
	close(listen_fd);
	return -2;
}

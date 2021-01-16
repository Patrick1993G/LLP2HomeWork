#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include "echo.h"

int main(int argc,char **argv) {
	char str[MAX_STR];
	int listen_fd, comm_fd;
	struct sockaddr_in servaddr;

	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_fd != -1) {
		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		servaddr.sin_port = htons(LISTEN_PORT);

		if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != -1) {
			if (listen(listen_fd, 10) != -1) {
				comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
				if (comm_fd != -1) {
					while(1) {
						memset(str, 0, MAX_STR);
						read(comm_fd,str, MAX_STR-1);
						printf("Echoing back - %s\n",str);
						write(comm_fd, str, strlen(str)+1);
					}
				}
			}
		}
	}
}

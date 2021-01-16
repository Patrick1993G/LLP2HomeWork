all: clean echo_client echo_server_multi

echo_server_multi: 
	gcc -Wall -o echo_server_multi echo_server_multi.c -lpthread

echo_client: 
	gcc -Wall -o echo_client echo_client.c

clean:
	-rm echo_client echo_server_multi
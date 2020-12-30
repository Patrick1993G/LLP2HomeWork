build_all: client server

rebuild_all: clean build_all

client:
	gcc -Wall -o client client.c

server:
	gcc -Wall -o server server.c

debug: 
	gcc -Wall -g -O0 -o client client.c 
	gcc -Wall -g -O0 -o server server.c 
release:

clean_all:
	-rm server client
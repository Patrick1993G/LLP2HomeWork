
# OS 	
RM = rm -rf #linux
RMDIR = rm -rf 
MKDIR = mkdir -p

build_all: debug release

rebuild_all: clean build_all

debug: 
	$(MKDIR) bin/dbg
	gcc -Wall -g -O0 -o bin/dbg/client_full -DDBGCLIENT client.c
	gcc -Wall -g -O0 -o bin/dbg/client_normal client.c
	gcc -Wall -g -O0 -o bin/dbg/server -DDBGSERVER server.c 

release:
	$(MKDIR) bin/rel
	gcc -o bin/rel/client_normal client.c
	gcc -o bin/rel/server -DSERVER server.c 
	gcc -o bin/rel/client_full -DFULL client.c
	
clean_all:
	$(RM) bin
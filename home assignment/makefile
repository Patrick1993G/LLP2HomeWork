
# OS 
ifeq ($(OS),Windows_NT) #for win
    RM = del /F /Q 
    RMDIR = -RMDIR /S /Q
    MKDIR = -mkdir
else  				
    RM = rm -rf #linux
    RMDIR = rm -rf 
    MKDIR = mkdir -p
endif

build_all: debug release

rebuild_all: clean build_all

debug: 
	$(MKDIR) bin/dbg
	gcc -Wall -g -O0 -o bin/dbg/client client.c
	gcc -Wall -g -O0 -o bin/dbg/server server.c 

release:
	$(MKDIR) bin/rel
	gcc -o bin/rel/client client.c
	gcc -o bin/rel/server server.c 
	
clean_all:
	-rm server client 
	$(RM) bin
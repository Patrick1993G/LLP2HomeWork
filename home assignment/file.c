#include "files.h"
void writeFile(char whereTo,char* toWrite){
    switch (whereTo)
    {
    case 'c':
        /* To client*/
        #if defined(DBG)
            char*path = "./bin/dbg/log.txt";
        #endif
        char*path = "./bin/rel/log.txt";
        writeToFile(path,toWrite);
        break;
    case 's':
        /*To server*/
        #if defined(DBG)
        char*path = "./bin/dbg/server.data";
        #endif
        char*path = "./bin/rel/server.data";
        writeToFile(path,toWrite);
        break;
    default:
        break;
    }
}
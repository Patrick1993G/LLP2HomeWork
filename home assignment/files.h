#ifndef files
#define files
//opening a file
FILE *openFile(char *path, char *symbol)
{
  
    FILE *fp = fopen(path, symbol);
    if (fp == NULL)
    {
        fprintf(stderr, "Can't open input file!\n");
    }
    else
    {
        fprintf(stdout, "Successfully opened file!\n");
    }
    return fp;
}
//Remove file
bool removeFile(char path[50])
{

    if (remove(path) == 0)
    {
        printf("All values are cleared\n");
        return true;
    }
    else
    {
        printf("File could not be deleted!\n");
        return false;
    }
}
//Writing
void writeToFile(char *path, char *toWrite)
{  printf("%s",path);
    FILE *fp = openFile(path, "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s\n", toWrite);
        fclose(fp);
    }
}
void writeFile(char whereTo, char *toWrite, char *clientPath)
{
    char path[50];
    memset(path, 0, 50);
    switch (whereTo)
    {
    case 'c':
        /* To client*/
        writeToFile(clientPath, toWrite);
        break;
    case 's':
        /*To server*/
#if defined(DBGSERVER)
        strcpy(path, "./bin/dbg/server.data");
#else
        strcpy(path, "./bin/rel/server.data");
#endif
        writeToFile(path, toWrite);
        break;
    default:
        printf("can not Write to file!\n");
        break;
    }
}
//Reading
void readFile(char *path)
{
    int MOISTURE;
    int PH;
    int SUNLIGHT;
    char STATS[200];
    FILE *fp = openFile(path, "r");
    if (fp != NULL)
    {
        //file opened
        while (!feof(fp))
        {
            //reading...
            if (fscanf(fp, "%d %d %d %s", &MOISTURE, &PH, &SUNLIGHT, STATS) != 4)
                break; //file format mismatch
            printf("\nRead: %d\t%d\n%d\t%s", MOISTURE, PH, SUNLIGHT, STATS);
        }
        //close file
        fclose(fp);
    }
}

#endif
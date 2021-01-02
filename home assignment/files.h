#ifndef files
#define files
//opening a file
FILE *openFile(char *path, char *symbol)
{
    FILE *fp = fopen(path, symbol);
    if (fp == NULL)
    {
        fprintf(stderr, "\nCan't open input file!\n");
    }
    else
    {
        fprintf(stdout, "\nSuccessfully opened file!\n");
    }
    return fp;
}
//Writing
void writeToFile(char *path, char *toWrite)
{
    FILE *fp = openFile(path, "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s\n", toWrite);
        fclose(fp);
    }
}
void writeFile(char whereTo, char *toWrite)
{
    char path[50];
    memset(path, 0, 50);
    switch (whereTo)
    {
    case 'c':
/* To client*/
#if defined(DBG)
        strcpy(path, "./bin/dbg/log.txt");
#else
        strcpy(path, "./bin/rel/log.txt");
#endif
        writeToFile(path, toWrite);
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
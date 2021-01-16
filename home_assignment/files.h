#ifndef files
#define files
//opening a file
FILE *openFile(char *path, char *symbol)
{
    FILE *fp;
    fp = fopen(path, symbol);
    printf("path to file %s\n", path);
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
bool removeFile(char *path)
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
{
    FILE *fp;
    fp = openFile(path, "a");
    printf("%s\n", path);
    printf("%s\n", toWrite);

    if (fp != NULL)
    {
        fprintf(fp, "%s\n", toWrite);
        printf("File updated\n");
        fclose(fp);
    }
    else
    {
        perror("Error");
    }
}
void writeFile(char whereTo, char *toWrite, char *clientPath)
{
    char *path = (char *)malloc(sizeof(char) * 50);
    if (path == NULL)
    {
        fprintf(stderr, "error !\n");
    }
    printf("Writing to file...\n");
    switch (whereTo)
    {
    case 'c':
        /* To client*/
        writeToFile(clientPath, toWrite);
        printf("client path %s", clientPath);
        break;
    case 's':
        strcpy(path, "./server.data");
        writeToFile(path, toWrite);
        break;
    default:
        printf("can not Write to file!\n");
        break;
    }
    free(path);
    path = NULL;
}
//Reading
void readFile(char *path, sensor *recSensData)
{
    FILE *fp = openFile(path, "r");
    if (fp != NULL)
    {
        //file opened
        while (!feof(fp))
        {
            //reading...
            if (fscanf(fp, "%d %d %d ", &recSensData->MOISTURE, &recSensData->PH, &recSensData->SUNLIGHT) != 3)
            {
                printf("error reading file!");
                break; //file format mismatch
            }

            printf("\nReading: %d\t%d\n%d", recSensData->MOISTURE, recSensData->PH, recSensData->SUNLIGHT);
            add_sensor_at_end(recSensData);
        }
        printf("Data filled from file !");
        //close file
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "There is nothing in the file!\n");
    }
}

#endif
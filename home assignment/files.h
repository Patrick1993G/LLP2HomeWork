
//opening a file
FILE* openFile(char *symbol)
{
    FILE *fp = fopen("sensor.txt", symbol);
    if (fp == NULL)
    {
        fprintf(stderr, "Can't open input file!\n");
    }
    else
    {
        fprintf(stdout, "Successfully opened file!\n");
        fclose(fp);
    }
    return fp;
}

//Reading
void readFile()
{
    int MOISTURE;
    int PH;
    int SUNLIGHT;
    char STATS[200];
	FILE *fp = openFile("r");
    if (fp != NULL) {
    //file opened
    while (!feof(fp)) {
        //reading...
        if (fscanf(fp, "%d %d %d %s", &MOISTURE, &PH, &SUNLIGHT, STATS) != 4)
            break; //file format mismatch
        printf("Read: %d\t%d\n%d\t%s",MOISTURE, PH, SUNLIGHT, STATS);
    }
     //close file
    fclose(fp);
	}
}

//Writing
void writeToFile(char* toWrite)
{
	FILE *fp = openFile("w");
	if (fp != NULL) {
		fprintf(fp, "%s\n", toWrite);
		fclose(fp);
	}
	
}
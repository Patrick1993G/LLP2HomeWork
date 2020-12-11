
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char* grader(int num){
    char* grade = malloc(sizeof(char)*10);
    if(num>0 && num<50){
        strcpy(grade,"F");
    }else if(num>49 && num<65){
        strcpy(grade,"C");
    }else if(num>64 && num<75){
        strcpy(grade,"B");
    }else if(num>74 && num<90){
        strcpy(grade,"A");
    }else if(num>89 && num<101){
        strcpy(grade,"A+");
    }
    return grade;
}
int main(int argc, char const *argv[])
{
    int  num;
    char string [20];
	FILE *fp;
    char toWrite [200];
    char toAppend [40];
	fp = fopen(argv[1], "r");
    
	if (fp != NULL) {

        printf("Processing rows....\n");
		while (!feof(fp)) {
			if (fscanf(fp, "%s %d",string,&num) != 2)
				break;
            
            sprintf(toAppend,"%s,%d,%s \n",string,num,grader(num));
            strcat(toWrite,toAppend);
			printf("Read: %s",toAppend);
            }
		 fclose(fp);
         printf("\nFinished all file, results in outputfile.csv");
	}
   
    fp = fopen("outputfile.csv", "w");
	if(fp != NULL){
         printf("\nWriting... \n%s",toWrite);
         fprintf(fp, "%s", toWrite);
    }
    fclose(fp);
    return 0;
}

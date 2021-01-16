
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int  num;
	FILE *fp;

	fp = fopen(argv[1], "r");
    int count = 0;
    int counter =0;
	if (fp != NULL) {
		while (!feof(fp)) {
			if (fscanf(fp, "%d",&num) != 1)
				break;
            
			printf("Read: %d\n",num);
            if(num >49){
                count++;
            }
            counter++;
		}
        printf("Pass rate is %.0f from %d",(float)count/counter*100,count);
		fclose(fp);
	}
   
    return 0;
}

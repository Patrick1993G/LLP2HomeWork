#include <stdio.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

int main(void)
{
	char buffer[11];
	int correct_password = FALSE;
	
	printf("Enter password: ");
	fgets(buffer,11,stdin);
	
	if (strcmp(buffer, "letmein") == 0)
		correct_password = TRUE;
	else
		printf ("Incorrect Password!\n");
	
	if (correct_password != FALSE)
		printf ("Performing tasks that require password...\n");

	return 0;
}
#include "threads.h"
#include <string.h>
#include <stdbool.h>

void* run_thread(void *ptr);

char* out_file;
char* in_file = NULL;

char name[NAME_SIZE];
int delay, maximum;

bool check_params(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: threads output [input]\n");
		return false;
	}

	out_file = argv[1];

	if (argc == 3)
		in_file = argv[2];
	
	return true;
}

bool spawn_thread(char* name, unsigned int delay, unsigned int maximum)
{
	thread_info* info = (thread_info*)malloc(sizeof(thread_info));
	if (info)
	{
		pthread_t thrd;
		strncpy(info->name, name, NAME_SIZE);
		info->delay = delay;
		info->maximum = maximum;
		pthread_create(&thrd, NULL, run_thread, info);
		return true;
	}

	return false;
}

void process_file()
{
	FILE *fp;

	fp = fopen(in_file, "r");
	if (fp != NULL)
	{
		while (!feof(fp)) {
			if (fscanf(fp, "%s %d %d", name, &delay, &maximum) != 3)
				break;

			printf("read %s,%d,%d\n", name, delay, maximum);
			spawn_thread(name, delay, maximum);
		}

		fclose(fp);
		printf("Press any key to exit\n");
		getchar();
	}
	else
	{
		printf("Failed to open %s\n", in_file);
	}
}

void process_userinput()
{
	while (true) 
	{
		printf("Enter the name of the thread: ");
		scanf("%s", name);
		if (strcmp(name, "stop") == 0)
			break;
		printf("Enter the delay in seconds: ");
		scanf("%d", &delay);
		printf("Enter the maximum number of writes: ");
		scanf("%d", &maximum);
		spawn_thread(name, delay, maximum);
	}
}

int main(int argc, char **argv)
{
	if (!check_params(argc, argv))
		return -1;

	pthread_mutex_init(&out_file_mutex, NULL);
	
	if (in_file)
		process_file();
	else
		process_userinput();

	pthread_mutex_destroy(&out_file_mutex);

	return 0;
}

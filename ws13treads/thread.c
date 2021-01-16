#include "threads.h"
#include <unistd.h> /* sleep */

char* out_file;

void* run_thread(void *ptr)
{
	thread_info* info = (thread_info*)ptr;

	for (int i=0; i<info->maximum; i++)
	{
		sleep(info->delay); // seconds
		pthread_mutex_lock(&out_file_mutex);
				
		FILE* fp = fopen(out_file, "a");
		if (fp)
		{
			fprintf(fp, "thread: %s \n", info->name);
			fclose(fp);
		}
		else
		{
			printf("File locked for thread %s\n", info->name);
		}

		pthread_mutex_unlock(&out_file_mutex);
	}

	free(ptr);
	return NULL;
}

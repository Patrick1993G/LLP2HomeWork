#ifndef THREADS_H_INCLUDED
#define THREADS_H_INCLUDED

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NAME_SIZE 32

pthread_mutex_t out_file_mutex;

typedef struct thread_t {
	char name[NAME_SIZE];
	unsigned int delay;
	unsigned int maximum;
} thread_info;

#endif //THREADS_H_INCLUDED
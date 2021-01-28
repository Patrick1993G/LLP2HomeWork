#ifndef THREADS_H_INCLUDED
#define THREADS_H_INCLUDED

#include <pthread.h>

//create the file mutex lock
pthread_mutex_t file_mutex;

typedef struct thread {
	int*comm;
	sensor *clientSensor;
	unsigned int id;
} client_thread;

#endif 
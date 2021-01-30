#ifndef THREADS_H_INCLUDED
#define THREADS_H_INCLUDED

#include <pthread.h>

//create the file mutex lock
pthread_mutex_t file_mutex;
pthread_mutex_t data_mutex;

#endif 
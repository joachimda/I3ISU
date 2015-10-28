#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include "Vector.hpp"

void* writer(void * arg);

Vector v;
int sleepTime = 1;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	int numOfThreads = 100;
	pthread_t threads[numOfThreads];

	std::cout << "Sleep is set to: " << sleepTime << " us." << std::endl;
	sleep(3);

	int i = 0; // i is sent as arg to thread, so it must be out of "for"-scope
	std::cout << "Creating threads..\n";
	for(i = 0; i < numOfThreads; i++) 
	{
		pthread_create(&threads[i], NULL, writer, (void*)&i);
		usleep(sleepTime);
	}

	std::cout << "joining threads..\n";
	for(i = 0; i < numOfThreads; i++)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void * writer(void *arg) 
{
	int id = *(int*)arg;

	while(1)
	{
		usleep(sleepTime);
		pthread_mutex_lock(&mtx);
		if (!v.setAndTest(id))
		{
			std::cout << "Failure from thread: " << id << std::endl;
		}
		else 
		{
			std::cout << "succes from thread: " << id << std::endl;
		}
		pthread_mutex_unlock(&mtx);
	}
}


#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include "Vector.hpp"

void* writer(void * arg);

Vector v;
int sleepTime = 1;

sem_t sem;

int main()
{
	int numOfThreads = 100;
	pthread_t threads[numOfThreads];

	int valOfSemaphore = 1; // = 1, working like mutex
	sem_init(&sem, 0, valOfSemaphore);

	std::cout << "Sleep is set to: " << sleepTime << " us." << std::endl;
	sleep(3);

	int i = 0;

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
	int id = *(int *)arg;
	while(1)
	{
		usleep(sleepTime);

		sem_wait(&sem);
		if (!v.setAndTest(id))
		{
			std::cout << "Failure from thread: " << id << std::endl;
		}
		else 
		{
			std::cout << "Succes from thread: " << id << std::endl;
		}
		sem_post(&sem);
	}
}


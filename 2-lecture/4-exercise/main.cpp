#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include "Vector.hpp"

void* writer(void * arg);

Vector v;
int sleepTime = 10000;

int main() {
	std::cout << "Sleep is set to: " << sleepTime << " us." << std::endl;
	sleep(3);

	int numOfThreads = 10;
	pthread_t threads[numOfThreads];

	int i = 0;

	std::cout << "Creating threads..\n";
	for(i = 0; i < numOfThreads; i++) {
		pthread_create(&threads[i], NULL, writer, (void*)&i);
		usleep(10000);
	}
	std::cout << "joining threads..\n";
	for(i = 0; i < numOfThreads; i++){
		pthread_join(threads[i], NULL);
	}
	return 0;
}

void * writer(void *arg) {
	int id = *(int *)arg;
	while(1){
		usleep(sleepTime);
		if (!v.setAndTest(id)){
			std::cout << "Failure from thread: " << id << std::endl;
		}
		else {
			std::cout << "succes from thread: " << id << std::endl;
		}
	}
}


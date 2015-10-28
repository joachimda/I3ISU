#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include "Vector.hpp"

void* writer(void * arg);
Vector v;

int main() {

	int numOfThreads = 100;
	pthread_t threads[numOfThreads];

	int i = 0;

	std::cout << "Creating threads..\n";
	for(i = 0; i < numOfThreads; i++) {
		pthread_create(&threads[i], NULL, writer, (void*)&i);
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
		sleep(1);
		if (!v.setAndTest(id)){
			std::cout << "Failure from thread: " << id << std::endl;
		}
		else {
			std::cout << "succes from thread: " << id << std::endl;
		}
	}
}


// main.cpp
#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

// standard prototype
void* func(void* ptr);

int main() {
	// declare variables
	int number_of_threads = 2;
	pthread_t threads[number_of_threads];
	int id_1 = 1, id_2 = 2;

	// create threads
	cout << "Main: Creating threads..." << endl;
	pthread_create(&threads[0], NULL, func, (void*)&id_1);
	pthread_create(&threads[1], NULL, func, (void*)&id_2);

	// join threads
	cout << "Main: Waiting for threads to finish..." << endl;
	for(int i = 0; i < number_of_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	cout << "Main: Exiting." << endl;
	return 0;
}

void* func(void* ptr) {
	int id = *(int*)ptr;

	for(int i = 0; i < 10; i++)	{
		cout << "Hello #" << i << " from thread " << id << endl;
		sleep(1);
	}

	pthread_exit(0);
}

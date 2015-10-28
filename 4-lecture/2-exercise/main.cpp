#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

// thread function prototypes
void* car(void* ptr);
void* entry_guard(void*ptr);
void* exit_guard(void*ptr);

// global variables
bool carWaitingEntry = false;
bool carWaitingExit = false;
bool enterOpen = false;
bool exitOpen = false;
int  maxSlots = 10;
int  curCars = 0;

pthread_mutex_t enter_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t exit_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t slots_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t entry_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t exit_cond = PTHREAD_COND_INITIALIZER;

int main() {
	// creating x number of car-threads
	int number_of_cars = 1;
	pthread_t threads[number_of_cars];

	// derp stuff
	bool youWantMainLogsPrinted = false;
	char userChoice;
	int userNumber;

	cout << "\nWould you like main logs to be printed? (y/n)" << endl;
	cin >> userChoice;
	if(userChoice == 'y')
		youWantMainLogsPrinted = true;

	cout << "How many slots would you like in the lot?" << endl;
	cin >> userNumber;
	if(userNumber > 0)
		maxSlots = userNumber;
	else
		cout << "Nope. You now get 10 slots." << endl;

	cout << "How many cars would you like?" << endl;
	cin >> userNumber;
	if(userNumber > 0)
		number_of_cars = userNumber;
	else
		cout << "Nope. You now get 1 car." << endl;

	cout << "\n";
	// done with derp stuff

	// starting actual part of program...
	if(youWantMainLogsPrinted)
		cout << "Main:\tCreating guards." << endl;
	pthread_t entryGuard;
	pthread_t exitGuard;
	pthread_create(&entryGuard, NULL, entry_guard, NULL);
	pthread_create(&exitGuard, NULL, exit_guard, NULL);

	if(youWantMainLogsPrinted)
		cout << "Main:\tCreating " << number_of_cars << " car thread(s)." << endl;
	int i;
	for(i = 0; i < number_of_cars; i++) {
		pthread_create(&threads[i], NULL, car, (void*)&i);
		usleep(1);
	}

	if(youWantMainLogsPrinted)
		cout << "Main:\tJoining " << number_of_cars << " car threads." << endl;
	for(i = 0; i < number_of_cars; i++) {
		pthread_join(threads[i], NULL);
	}

	if(youWantMainLogsPrinted)
		cout << "Main:\tJoining guards." << endl;
	pthread_join(entryGuard, NULL);
	pthread_join(exitGuard, NULL);

	if(youWantMainLogsPrinted)
		cout << "Main:\tReturning." << endl;
	return 0;
}

void* car(void* ptr) {
	int id = *(int*)ptr;

	while(true) {
		// car entering...
		pthread_mutex_lock(&enter_mtx);
		cout << "Car" << id << ":\tDrives up to entry." << endl;
		carWaitingEntry = true;
		pthread_cond_signal(&entry_cond);
		while(!enterOpen) {
			pthread_cond_wait(&entry_cond, &enter_mtx);
		}
		cout << "Car" << id << ":\tDrives into parkinglot." << endl;	
		carWaitingEntry = false;
		pthread_cond_signal(&entry_cond);
		pthread_mutex_unlock(&enter_mtx);
		// car inside parking lot

		cout << "Car" << id << ": \tIs parked." << endl;
		usleep((id+1)*100000);	// car must wait unique amount of time
		cout << "Car" << id << ": \tWants to leave." << endl;

		// car exiting...
		pthread_mutex_lock(&exit_mtx);
		cout << "Car" << id << ": \tDrives up to exit." << endl;
		carWaitingExit = true;
		pthread_cond_signal(&exit_cond);
		while(!exitOpen) {
			pthread_cond_wait(&exit_cond, &exit_mtx);
		}
		cout << "Car" << id << ": \tDrives out of parkinglot." << endl;
		carWaitingExit = false;
		pthread_cond_signal(&exit_cond);
		pthread_mutex_unlock(&exit_mtx);
		// car outside parking lot

		sleep(3);	// car waiting before reentering
	}
}

void* entry_guard(void* ptr) {
	while(true){
		pthread_mutex_lock(&enter_mtx);
		pthread_mutex_lock(&slots_mtx);
		do{
			while(!carWaitingEntry) {
				pthread_cond_wait(&entry_cond, &enter_mtx);
			}
		}while(curCars > maxSlots);

		curCars++;
		pthread_mutex_unlock(&slots_mtx);

		cout << "Entry:\tOpening entry-door." << endl;
		enterOpen = true;
		pthread_cond_signal(&entry_cond);
		while(carWaitingEntry) {
			pthread_cond_wait(&entry_cond, &enter_mtx);
		}
		cout << "Entry:\tClosing entry-door." << endl;
		enterOpen = false;
		pthread_mutex_unlock(&enter_mtx);
	}
}

void* exit_guard(void* ptr) {
	while(true){
		pthread_mutex_lock(&exit_mtx);
		pthread_mutex_lock(&slots_mtx);
		while(!carWaitingExit) {
			pthread_cond_wait(&exit_cond, &exit_mtx);
		}

		if(curCars > 0) {
			curCars--;
		}
		pthread_mutex_unlock(&slots_mtx);

		cout << "Exit:\tOpening exit-door." << endl;
		exitOpen = true;
		pthread_cond_signal(&exit_cond);
		while(carWaitingExit) {
			pthread_cond_wait(&exit_cond, &exit_mtx);
		}
		cout << "Exit:\tClosing exit-door." << endl;
		exitOpen = false;
		pthread_mutex_unlock(&exit_mtx);
	}
}
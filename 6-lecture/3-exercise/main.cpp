// main.cpp
#include <iostream>
#include <string>
#include <osapi/MsgQueue.hpp>
#include "threads.hpp"

int main() {

	int numOfCars = 5;

	/*guards*/
	MsgQueue entryQueue(numOfCars);
	MsgQueue exitQueue(numOfCars);

	EntryGuardThread entryGuardThread(&entryQueue);
	ExitGuardThread exitGuardThread(&exitQueue);

	Thread entryGuard(&entryGuardThread);
	Thread exitGuard(&exitGuardThread);

	entryGuard.start();
	exitGuard.start();

	std::queue<Thread*> cars;
	std::queue<CarThread*> carThreads;

	for (int i = 0; i < numOfCars; i++)
	{
		CarData_s *data = new CarData_s;
		data->entryQ = &entryQueue;
		data->exitQ = &exitQueue;
		data->ID = i;
		CarThread *carThread = new CarThread(data);
		Thread *car = new Thread(carThread);
		cars.push(car);
		carThreads.push(carThread);
		car->start();
	}

	entryGuard.join();
	exitGuard.join();

	while(!cars.empty())
	{
		Thread *car = cars.front();
		cars.pop();
		car->join();
		delete car;
	}

	while (!carThreads.empty())
	{
		CarThread *thread = carThreads.front();
		carThreads.pop();
		delete thread;
	}

	return 0;
}
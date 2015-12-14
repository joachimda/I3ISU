// main.cpp
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include "Point3D.h"
#include "MsgQueue.h"
using namespace std;

// Prototypes
void* sender  (void* ptr);	//ThreadFunc
void* receiver(void* ptr);	//ThreadFunc
void printPoint(Point3D* point);
void messageHandler(unsigned long mId, Message* msg);

// Message types
enum {
	POINT_CREATED,
	SOME_SOTHER_MESSAGE
};

// Global MsgQueue
MsgQueue myMsgQueue(20);

int main() 
{
	int numOfThreads = 2;

	// Create threads
	cout << "Main: Making " << numOfThreads << " number of threads..." << endl;
	pthread_t threads[numOfThreads];

	// Start threads
	cout << "Main: Creating threads..." << endl;
	pthread_create(&threads[0], NULL, sender  , &myMsgQueue);
	pthread_create(&threads[1], NULL, receiver, &myMsgQueue);

	// Join threads
	cout << "Main: Joining threads..." << endl;
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	cout << "Main: Main returning..." << endl;
	return 0;
}

void* sender(void* ptr)
{
	MsgQueue *myMsgQueue = static_cast<MsgQueue*>(ptr);
	int id = 0, x = 0, y = 0, z = 0;

	// loop every second
	while(true)
	{
		// Point3D* myPoint = new Point3D;
<<<<<<< HEAD
		Point3D* myPoint = new Point3D;
		myPoint->x = x;
		myPoint->y = y;
		myPoint->z = z;

		myMsgQueue->send(id, myPoint);

		cout << "Sender: Sent Point(" << myPoint->x << "," << myPoint->y << "," << myPoint->z << ")!" << endl;

		x++;
		y++;
		z++;
		id++;
		usleep(100000);
=======
		Point3D* point = new Point3D;
		int x = rand() %100;
		int y = rand() %100;
		int z = rand() %100;
		point->x = x;
		point->y = y;
		point->z = z;

		myMsgQueue->send(POINT_CREATED, point);
		cout << "Sender threads puts data into queue: " << endl;
		sleep(1);
	}
}

void messageHandler(unsigned long mId, Message* msg)
{
	switch(mId)
	{
		case POINT_CREATED:
		{
			Point3D* point = static_cast<Point3D *>(msg);
			printPoint(point);
		}
		case SOME_SOTHER_MESSAGE:
		{
			break;
		}
		default:
		{
			cout << "An error has ocurred..\n";
			break;
		}
>>>>>>> eac66d776d06e9f48cd81fdbd636d8b67aeac6bf
	}
}

void printPoint(Point3D* point)
{
	cout << "\nX-Coord: " << point->x << "\nY-Coord: " << point->y << "\nZ-Coord: " << point->z << endl;
}

void* receiver(void* ptr)
{
	MsgQueue* myMsgQueue = static_cast<MsgQueue*>(ptr);

	// wait for new point
	while(true)
	{
<<<<<<< HEAD
		unsigned long id;
		Message* msg = myMsgQueue->recieve(id);
		Point3D* receivedPoint = static_cast<Point3D*>(msg);

		cout << "Receiver: Got ID: " << id << ", with Point(" << receivedPoint->x << "," << receivedPoint->y << "," << receivedPoint->z << ")!" << endl;
=======
		unsigned long mId;
		Message* msg = myMsgQueue->recieve(mId);
>>>>>>> eac66d776d06e9f48cd81fdbd636d8b67aeac6bf

		// Handle the message
		messageHandler(mId, msg);
		delete msg;
	}
}
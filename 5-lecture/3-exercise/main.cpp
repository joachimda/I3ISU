// main.cpp
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include "MsgQueue.h"
using namespace std;

// Message types
enum {
	ACK_ENTRY_OPEN = 1,
	ACK_ENTRY_CLOSE,
	ACK_EXIT_OPEN,
	ACK_EXIT_CLOSE,
	REQ_ENTRY_OPEN,
	REQ_ENTRY_CLOSE,
	CAR_PARKED,
	CAR_LEAVING
};

// Message queues
MsgQueue entryMessageQueue(10), exitMessageQueue(10);

// Datacontainer for car
struct data_s : public Message {
	/* data */
    int car_id;
    MsgQueue* car_messageQueue;
};

// Damn messages!
struct entryOpenMessage 			: Message { int car_id; MsgQueue* from; };
struct entryClosedMessage 			: Message { int car_id; MsgQueue* from; };
struct exitOpenMessage				: Message { int car_id; MsgQueue* from; };
struct exitClosedMessage			: Message { int car_id; MsgQueue* from; };
struct entryOpenRequestMessage		: Message { int car_id; MsgQueue* from; };
struct entryClosedRequestMessage	: Message { int car_id; MsgQueue* from; };
struct goingToParkMessage			: Message { int car_id; MsgQueue* from; };
struct leavingLotMessage			: Message { int car_id; MsgQueue* from; };

// Prototypes
static void* messageHandlerFunction(Message* msg, unsigned long id);
static void* carThreadFunction(void* idPtr);
static void* entryGuardThreadFunction(void* idPtr);
static void* exitGuardThreadFunction(void* idPtr);

int main() {
	// creating x number of car-threads
	int numberOfCars = 1, userNumber, i;
	pthread_t threads[numberOfCars], entryGuard, exitGuard;
	bool youWantMainLogsPrinted = false;
	char userChoice;

	cout << "\nWould you like main logs to be printed? (y/n) ";
	cin >> userChoice;
	if(userChoice == 'y') youWantMainLogsPrinted = true;
	cout << "How many cars would you like? ";
	cin >> userNumber;
	if(userNumber > 0) numberOfCars = userNumber;
	else cout << "Nope. You now get 1 car." << endl;

	// starting actual part of program...
	if(youWantMainLogsPrinted) cout << "Main:\tCreating guards." << endl;
	pthread_create(&entryGuard, NULL, entryGuardThreadFunction, NULL);
	pthread_create(&exitGuard , NULL, exitGuardThreadFunction , NULL);

	if(youWantMainLogsPrinted) cout << "Main:\tCreating " << numberOfCars << " car thread(s)." << endl;
	for(i = 0; i < numberOfCars; i++) {
		data_s* car = new data_s;
		car->car_id = i;
		car->car_messageQueue = new MsgQueue(1);
		pthread_create(&threads[i], NULL, carThreadFunction, (void*)car);
	}

	if(youWantMainLogsPrinted) cout << "Main:\tJoining " << numberOfCars << " car threads." << endl;
	for(i = 0; i < numberOfCars; i++) {
		pthread_join(threads[i], NULL);
	}

	if(youWantMainLogsPrinted) cout << "Main:\tJoining guards." << endl;
	pthread_join(entryGuard, NULL);
	pthread_join(exitGuard, NULL);

	if(youWantMainLogsPrinted) cout << "Main:\tReturning." << endl;
	return 0;
}

static void* carThreadFunction(void* idPtr)
{
	// deal with cast first
    data_s* car = static_cast<data_s*>(idPtr);

    // make request
    entryOpenRequestMessage* open_Req = new entryOpenRequestMessage;
    open_Req->car_id = car->car_id;
    open_Req->from = car->car_messageQueue;
    
    // send initial req
    car->car_messageQueue->send(REQ_ENTRY_OPEN, open_Req);
    
    // commence loop
    while(true)
    {
        unsigned long id;
        Message* msg = car->car_messageQueue->receive(id);
        messageHandlerFunction(msg, id);
        delete msg;
    }
}

static void* entryGuardThreadFunction(void* idPtr)
{
	// loop
    while(true)
    {
    	// deal with msg
        unsigned long id;
        Message* msg = entryMessageQueue.receive(id);
        messageHandlerFunction(msg, id);
        delete msg;
    }
}

static void* messageHandlerFunction(Message* msg, unsigned long id) {
	switch(id) {
		case ACK_ENTRY_OPEN: {
			// cast message
			entryOpenMessage* entry_Open = static_cast<entryOpenMessage*>(msg);

			// print event
			cout << "Entry open." << endl;

			// reply
			entryClosedRequestMessage* entry_CloseReq = new entryClosedRequestMessage;
			entry_CloseReq->car_id = entry_Open->car_id;
			entry_CloseReq->from = entry_Open->from;
			entry_Open->from->send(REQ_ENTRY_CLOSE, entry_CloseReq);
			break;
		}
		case ACK_ENTRY_CLOSE: {
			entryClosedMessage* entry_Close = static_cast<entryClosedMessage*>(msg);
			cout << "Entry closed." << endl;
			goingToParkMessage* go_Park = new goingToParkMessage;
			go_Park->car_id = entry_Close->car_id;
			go_Park->from = entry_Close->from;
			entry_Close->from->send(CAR_PARKED, go_Park);
			break;
		}
		case ACK_EXIT_OPEN: {
			exitOpenMessage* exit_Open = static_cast<exitOpenMessage*>(msg);
			cout << "Exit open." << endl;

			leavingLotMessage* leave_PLCS = new leavingLotMessage;
			leave_PLCS->car_id = exit_Open->car_id;
			leave_PLCS->from = exit_Open->from;
			exit_Open->from->send(CAR_LEAVING, leave_PLCS);
			break;
		}
		case ACK_EXIT_CLOSE: {
			exitClosedMessage* exit_Close = static_cast<exitClosedMessage*>(msg);
			cout << "Exit closed." << endl;

			// get random time to sleep in
			int shortestTimeToWait = 1;
			int longestTimeToWait = 10;
			int timeToSleep = rand() % longestTimeToWait + shortestTimeToWait;
			sleep(timeToSleep);

			entryOpenRequestMessage* entry_OpenReq = new entryOpenRequestMessage;
			entry_OpenReq->car_id = exit_Close->car_id;
			entry_OpenReq->from = exit_Close->from;
			exit_Close->from->send(REQ_ENTRY_OPEN, entry_OpenReq);
			break;
		}
		case REQ_ENTRY_OPEN: {
			entryOpenRequestMessage* open_Req = static_cast<entryOpenRequestMessage*>(msg);
			cout << "Car" << open_Req->car_id << ": Awaiting entry." << endl;
			entryOpenMessage* entry_Open = new entryOpenMessage;
			entry_Open->car_id = open_Req->car_id;
			entry_Open->from = open_Req->from;
			entryMessageQueue.send(ACK_ENTRY_OPEN, entry_Open);
			break;
		}   
		case REQ_ENTRY_CLOSE: {
			entryClosedRequestMessage* entry_CloseReq = static_cast<entryClosedRequestMessage*>(msg);
			cout << "Car" << entry_CloseReq->car_id << ": Driving in..." << endl;
			entryClosedMessage* entry_Close = new entryClosedMessage;
			entry_Close->car_id = entry_CloseReq->car_id;
			entry_Close->from = entry_CloseReq->from;
			entryMessageQueue.send(ACK_ENTRY_CLOSE, entry_Close);
			break;
		}
		case CAR_PARKED: {
			goingToParkMessage* go_Park = static_cast<goingToParkMessage*>(msg);
			cout << "Car" << go_Park->car_id << ": Parked." << endl;

			// get random time to sleep in
			int shortestTimeToWait = 3;
			int longestTimeToWait = 6;
			int timeToSleep = rand() % longestTimeToWait + shortestTimeToWait;
			sleep(timeToSleep);

			cout << "Car" << go_Park->car_id << ": Exiting..." << endl;
			exitOpenMessage* exit_Open = new exitOpenMessage;
			exit_Open->car_id = go_Park->car_id;
			exit_Open->from = go_Park->from;
			exitMessageQueue.send(ACK_EXIT_OPEN, exit_Open);
			break;
		}
		case CAR_LEAVING: {
			leavingLotMessage* leave_PLCS = static_cast<leavingLotMessage*>(msg);
			cout << "Car" << leave_PLCS->car_id << ": has left to lot." << endl;
			exitClosedMessage* exit_Close = new exitClosedMessage;
			exit_Close->car_id = leave_PLCS->car_id;
			exit_Close->from = leave_PLCS->from;
			exitMessageQueue.send(ACK_EXIT_CLOSE, exit_Close);
			break;
		}
		default: {
			cout << "You done goofed!" << endl;
			break;
		}
	}
}
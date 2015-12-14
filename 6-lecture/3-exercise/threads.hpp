
// threads.hpp
#ifndef THREADS_HPP_
#define THREADS_HPP_

#include "messageTypes.hpp"
#include "handlesEntry.hpp"
#include "handlesExit.hpp"
#include "handlesCar.hpp"
#include <osapi/Thread.hpp>
#include <queue>

class EntryGuardThread : public ThreadFunctor
{
public: 
	EntryGuardThread(MsgQueue* entryQ) : messageQ(entryQ), state(GUARD_IDLE) {}
	virtual void run() 
	{
		while(1)
		{
			unsigned long id;
			Message* msg = messageQ->receive(id);
			entryHandleMessage(msg, id, state, pendingRequests);
		}
	}
private:
	MsgQueue* messageQ;
	GuardState state;
	std::queue<ReqDoorOpen_s*> pendingRequests;
};

class ExitGuardThread : public ThreadFunctor
{
public:
	ExitGuardThread(MsgQueue* exitQ)
		:messageQ(exitQ), state(GUARD_IDLE) {}
	
	virtual void run()
	{
		while(1)
		{
			unsigned long id;
			Message *msg = messageQ->receive(id);
			exitHandleMessage(msg, id, state, waitingReqs);
			delete msg;
		}
		delete messageQ;
	}
	virtual ~ExitGuardThread(){}

private:
	MsgQueue *messageQ;
	GuardState state;
	std::queue<ReqDoorOpen_s*> waitingReqs;
};

class CarThread : public ThreadFunctor
{
public:
	CarThread(CarData_s *data) : cardata(data){}
	virtual void run()
	{
		carMsgGenerate_REQ_ENTRY_OPEN(&cardata->carQ, cardata->entryQ, cardata->ID);

		while(1)
		{
			unsigned long id;
			Message *msg = (cardata->carQ).receive(id);
			carHandleMessage(id, msg, cardata);
			delete msg;
		}
		delete cardata;
	}
	virtual ~CarThread(){}
private:
	CarData_s* cardata; 
};

#endif
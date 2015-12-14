//handlesExit.hpp
#ifndef HANDLESEXIT_HPP_
#define HANDLESEXIT_HPP_

#include <queue>
#include <iostream>
#include "messageTypes.hpp"

void exitGenerate_CFM_ENTRY_OPEN(MsgQueue *messageQ)
{
	CfmDoorOpen_s* myCfm = new CfmDoorOpen_s;
	myCfm->result = true;
	messageQ->send(CFM_EXIT_OPEN, myCfm);
}

void exitHandle_REQ_EXIT_OPEN(ReqDoorOpen_s* request)
{
	std::cout << "Exit opening for car: " << request->ID << std::endl;
	exitGenerate_CFM_ENTRY_OPEN(request->mq_for_car);
}

void handleExitWaitingCars(std::queue<ReqDoorOpen_s*> &guardQ, GuardState &state)
{
	if (!guardQ.empty())
	{
		ReqDoorOpen_s *request = guardQ.front();
		guardQ.pop();
		exitHandle_REQ_EXIT_OPEN(request);
	}
	else
	{
		state = GUARD_IDLE;
	}
}

void handleExitReady(Message *msg, unsigned long id)
{
	switch(id)
	{
		case REQ_EXIT_OPEN:
		{
			ReqDoorOpen_s *request = static_cast<ReqDoorOpen_s*>(msg);
			exitHandle_REQ_EXIT_OPEN(request);
			break;
		}
		default:
			std::cout <<"ERR - exitHandler recieved a wrong message: " << id << std::endl;
	}
}

void handleExitBusy(Message *msg, unsigned long id, GuardState &state, std::queue<ReqDoorOpen_s*> &guardQ)
{
	switch(id)
	{
		case REQ_EXIT_OPEN:
		{
			ReqDoorOpen_s *request = static_cast<ReqDoorOpen_s*>(msg);
			guardQ.push(request);
			break;
		}
	}
}

void exitHandleMessage(Message *msg, unsigned long id, GuardState &state, std::queue<ReqDoorOpen_s*> &guardQ)
{
	switch(state)
	{
		case GUARD_IDLE:
			state = GUARD_BUSY;
			handleExitReady(msg, id);
			break;
		case GUARD_BUSY:
			handleExitBusy(msg, id, state, guardQ);
			break;
	}
}

#endif
// entryHandles.hpp
#ifndef ENTRYHANDLES_HPP_
#define ENTRYHANDLES_HPP_

#include <iostream>
#include "messageTypes.hpp"
#include <queue>

void entryGenerate_CFM_ENTRY_OPEN(MsgQueue* carQ)
{
	CfmDoorOpen_s* myCfm = new CfmDoorOpen_s;
	myCfm->result = true;
	carQ->send(CFM_ENTRY_OPEN, myCfm);
}

void entryHandle_ReqDoorOpen(ReqDoorOpen_s* req)
{
	std::cout << "Entry opening for car: " << req->ID << std::endl;
	entryGenerate_CFM_ENTRY_OPEN(req->mq_for_car);
}

void entryHandle_WaitingCars(std::queue<ReqDoorOpen_s*> &guardQ, GuardState &state)
{
	if(!guardQ.empty())
	{
		ReqDoorOpen_s* request = guardQ.front();
		guardQ.pop();
		entryHandle_ReqDoorOpen(request);
	}
	else
	{
		state = GUARD_IDLE;
	}
}

void handleEntryReady(Message* msg, unsigned long id)
{
	switch(id)
	{
		case REQ_ENTRY_OPEN:
		{
			ReqDoorOpen_s* req = static_cast<ReqDoorOpen_s*>(msg);
			entryHandle_ReqDoorOpen(req);
			break;
		}
		default:
			std::cout << "You done goofed!" << std::endl;
			break;
	}
}

void handleEntryBusy(Message* msg, unsigned long id, GuardState &state, std::queue<ReqDoorOpen_s*> &guardQ)
{
	switch(id)
	{
		case REQ_ENTRY_OPEN:
		{
			ReqDoorOpen_s* req = static_cast<ReqDoorOpen_s*>(msg);
			entryHandle_ReqDoorOpen(req);
			break;
		}
		default:
			std::cout << "You done goofed!" << std::endl;
			break;
	}
}

void entryHandleMessage(Message *msg, unsigned long id, GuardState &state, std::queue<ReqDoorOpen_s*> &guardQ)
{
	switch(state)
	{
		case GUARD_IDLE:
			state = GUARD_BUSY;
			handleEntryReady(msg, id);
			break;
		case GUARD_BUSY:
			handleEntryBusy(msg, id, state, guardQ);
			break;
	}
}

#endif
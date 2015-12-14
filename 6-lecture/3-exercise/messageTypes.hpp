// messageTypes.hpp
#ifndef MESSAGETYPE_HPP_
#define MESSAGETYPE_HPP_

#include <osapi/Message.hpp>
#include <osapi/MsgQueue.hpp>

using namespace osapi;

enum 
{
	REQ_ENTRY_OPEN,
	CFM_ENTRY_OPEN,
	REQ_EXIT_OPEN,
	CFM_EXIT_OPEN,
	CAR_INSIDE_LOT,
	CAR_OUTSIDE_LOT
};

enum GuardState
{
	GUARD_IDLE,
	GUARD_BUSY
};

struct CarData_s
{
	/* data */
	CarData_s() : carQ(1) {}
	MsgQueue carQ;
	MsgQueue* entryQ;
	MsgQueue* exitQ;
	int ID;
};

struct ReqDoorOpen_s : public Message
{
	/* data */
	MsgQueue* mq_for_car;
	int ID;
};

struct CfmDoorOpen_s : public Message
{
	/* data */
	bool result;
};

struct CarInsideLot_s : public Message
{
	/* data */
	int ID; 
};

struct CarOutsideLot_s : public Message
{
	/* data */
	int ID; 
};

#endif
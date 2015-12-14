// carHandles.hpp
#ifndef HANDLESCAR_HPP_
#define HANDLESCAR_HPP_

#include <iostream>
#include "messageTypes.hpp"

void carMsgGenerate_REQ_ENTRY_OPEN(MsgQueue* carQ, MsgQueue* entryQ, int id) 
{
	ReqDoorOpen_s* myReq = new ReqDoorOpen_s;
	myReq->mq_for_car = carQ;
	myReq->ID = id;
	entryQ->send(REQ_ENTRY_OPEN, myReq);
}

void carMsgGenerate_REQ_EXIT_OPEN(MsgQueue* carQ, MsgQueue* exitQ, int id) 
{
	ReqDoorOpen_s* myReq = new ReqDoorOpen_s;
	myReq->mq_for_car = carQ;
	myReq->ID = id;
	exitQ->send(REQ_EXIT_OPEN, myReq);
}

void carMsgGenerate_CAR_INSIDE_LOT(MsgQueue* entryQ, int id)
{
	CarInsideLot_s* myReq = new CarInsideLot_s;
	myReq->ID = id;
	entryQ->send(CAR_INSIDE_LOT, myReq);
}

void carMsgGenerate_CAR_OUTSIDE_LOT(MsgQueue* exitQ, int id)
{
	CarInsideLot_s* myReq = new CarInsideLot_s;
	myReq->ID = id;
	exitQ->send(CAR_OUTSIDE_LOT, myReq);
}

void car_WhileParked(int id)
{
	int sleepytime = rand() % 10 + 1;
	std::cout << "Car: " << id << " is parked." << std::endl;
	osapi::sleep(sleepytime*1000);
}

void car_WhileOutside(int id)
{
	int sleepytime = rand() % 10 + 1;
	std::cout << "Car: " << id << " is outside." << std::endl;
	osapi::sleep(sleepytime*1000);
}

void carHandleMessage(unsigned long id, Message* msg, CarData_s* data)
{
	switch(id)
	{
		case CFM_ENTRY_OPEN:
		{
			std::cout << "Car: " << id << " drives into lot." << std::endl;
			carMsgGenerate_CAR_INSIDE_LOT(&data->carQ, data->ID);
			car_WhileParked(data->ID);
			carMsgGenerate_REQ_EXIT_OPEN(&data->carQ, data->exitQ, data->ID);
			break;
		}
		
		case CFM_EXIT_OPEN:
		{
			std::cout << "Car: " << id << " exits lot." << std::endl;
			carMsgGenerate_CAR_OUTSIDE_LOT(&data->carQ, data->ID);
			car_WhileOutside(data->ID);
			carMsgGenerate_REQ_ENTRY_OPEN(&data->carQ, data->entryQ, data->ID);
			break;
		}
	}
}

#endif
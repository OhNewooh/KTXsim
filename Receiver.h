#pragma once
#include "DEVSim.h"
#include "message_def.h"
#include "engine_def.h"

class CReceiver : public CAtomic
{
public:
	CReceiver(int rec_id);
	~CReceiver();

public:
	static std::string inport_hum;
	static std::string inport_taxi;
	static std::string inport_next_collector;
	static std::string outport_decision;

public:
	virtual bool ExtTransFn(const CDEVSimMessage & _msg);
	virtual bool IntTransFn();
	virtual bool OutputFn(CDEVSimMessage & _msg);
	virtual TimeType TimeAdvanceFn();

	//state enum
public:
	typedef enum { WAIT, SEND } SysState;
	SysState m_state;

	RoadInfo* cur_road_info;
	int m_rec_id;
};
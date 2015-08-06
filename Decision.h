#pragma once
#include "DEVSim.h"
#include "message_def.h"
#include "engine_def.h"


class CDecision : public CAtomic
{
public:
	CDecision();
	~CDecision();

	static std::string inport;
	static std::string outport;

public:
	virtual bool ExtTransFn(const CDEVSimMessage & _msg);
	virtual bool IntTransFn();
	virtual bool OutputFn(CDEVSimMessage & _msg);
	virtual TimeType TimeAdvanceFn();

public:
	enum SysState { SEND, WAIT };
	SysState m_state;

public:
	RoadInfo* cur_road_info;
};

//decision은 현재 길의 정보를 받아서, 명령을 내려야 한다.
//그래서 roadInfo와 Orderinfo를 가지고 있다.
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

//decision�� ���� ���� ������ �޾Ƽ�, ����� ������ �Ѵ�.
//�׷��� roadInfo�� Orderinfo�� ������ �ִ�.
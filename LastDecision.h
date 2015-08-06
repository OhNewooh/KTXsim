#pragma once
#include "DEVSim.h"
#include "message_def.h"
#include <assert.h>
#include "engine_def.h"

/*
Last Decision은 매칭이 안 되면 GO를 할 수 없게 규칙만 바꿔준 Decision이다. 구조는 decision과 똑같다.

Last decision이 없이 그냥 decision을 붙일 경우,
RoadInfo가 담고 있는 NextHuman, NextTaxi는 agent_id가 -1로 초기화 되므로
다음 칸이 비었다고 생각하고 Agent들은 GO로 나가 줄줄이 사라져 버린다.

*/
class CLastDecision : public CAtomic
{
public:
	CLastDecision();
	~CLastDecision();

	static std::string inport;
	static std::string outport;

public:
	virtual bool ExtTransFn(const CDEVSimMessage & _msg);
	virtual bool IntTransFn();
	virtual bool OutputFn(CDEVSimMessage & _msg);
	virtual TimeType TimeAdvanceFn();

	//status
public:
	enum SysState { SEND, WAIT };
	SysState m_state;

public:
	RoadInfo* cur_road_info;
};


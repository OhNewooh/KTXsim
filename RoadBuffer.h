#pragma once
#include "DEVSim.h"
#include "message_def.h"
#include "engine_def.h"
#include <vector>
#include <assert.h>


class CRoadBuffer : public CAtomic
{
public:
	CRoadBuffer(int road_id, int road_type);
	~CRoadBuffer();


public:
	virtual bool ExtTransFn(const CDEVSimMessage &_msg);
	virtual bool IntTransFn();
	virtual bool OutputFn(CDEVSimMessage &_msg);
	virtual TimeType TimeAdvanceFn();

public:
	enum TState{ REPORT, SEND, WAIT };
	TState m_state;

public:
	int m_road_id; 
	int m_road_type; // 기존에 m_flag로 주던 것. 확장가능하라고 int로 바꿨다.

	std::vector<HumanInfo> m_human_list;
	std::vector<TaxiInfo> m_taxi_list;		


public:
	static std::string outport_obj;
	static std::string inport_obj;
	static std::string outport_coll;
	static std::string inport_coll;

	CellOrder command; // This Cell이 받은 명령
};
#pragma once
#include "DEVSim.h"

class CCollector : public CCoupled
{
public:
	CCollector(int coll_id);
	~CCollector();

public:
	static std::string inport_hum;
	static std::string inport_taxi;
	static std::string inport_next_collector;
	static std::string inport_decision;

	static std::string outport_hum;
	static std::string outport_taxi;
	static std::string outport_prev_collector;
	static std::string outport_decision;

public:
	int m_coll_id;
};

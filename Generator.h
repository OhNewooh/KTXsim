#pragma once
#include "DEVSim.h"
#include "Generator.h"
#include "message_def.h"
#include "engine_def.h"

class CGenerator : public CAtomic
{

public:
	CGenerator(char* model_name, int gen_type, double freq);
	~CGenerator();
	
public:
	virtual bool ExtTransFn(const CDEVSimMessage &_msg);
	virtual bool IntTransFn();
	virtual bool OutputFn(CDEVSimMessage &_msg);
	virtual TimeType TimeAdvanceFn();

public:
	static std::string outport;

public:
	enum Tstate{GEN};

private : 
	Tstate m_state;
	int m_generated_agent_cnt;
	int m_gen_type;
	double m_freq;



};


#include "Generator.h"

std::string CGenerator::outport = "outport";


CGenerator::CGenerator(char* model_name, int gen_type, double freq)
	: m_freq(freq), m_gen_type(gen_type), m_generated_agent_cnt(0)
{
	SetName(model_name);
	AddOutPorts(1, outport.c_str());
}


CGenerator::~CGenerator()
{
}

bool CGenerator::ExtTransFn(const CDEVSimMessage &_msg)
{
	return true;
}

bool CGenerator::IntTransFn()
{
	return true;
}

bool CGenerator::OutputFn(CDEVSimMessage &_msg)
{
	if (m_gen_type == HUMAN)
	{
		HumanInfo gen_agent;
		gen_agent.agent_id = m_generated_agent_cnt++;
		_msg.SetPortValue(outport.c_str(), new CHumanInfoMsg(gen_agent));

		char buf[100];
		sprintf_s(buf, 100, "%d Human have generated.\n", m_generated_agent_cnt);
		LOG((CAT_SHOW, LOG_INFO, buf));
	}

	else if (m_gen_type == TAXI)
	{
		TaxiInfo gen_agent;
		gen_agent.agent_id = m_generated_agent_cnt++;
		_msg.SetPortValue(outport.c_str(), new CTaxiInfoMsg(gen_agent));

		char buf[100];
		sprintf_s(buf, 100, "%d Taxi have generated.\n", m_generated_agent_cnt);
		LOG((CAT_SHOW, LOG_INFO, buf));
	}


	//_msg.SetPortValue(outport.c_str(), NULL);

	return true;
}

TimeType CGenerator::TimeAdvanceFn()
{
	return m_freq;
}
//m_freq is frequence
#include "ClockAtomic.h"

std::string CClockAtomic::outport_last_coll = "outport_last_coll";

CClockAtomic::CClockAtomic()
{
	SetName("ClockAtomic");
	AddOutPorts(1, outport_last_coll.c_str());

	m_round = 0;
	m_state = INIT;
}


CClockAtomic::~CClockAtomic()
{

}

bool CClockAtomic::ExtTransFn(const CDEVSimMessage &_msg)
{
	return true;
}

bool CClockAtomic::IntTransFn()
{
	return true;
}

bool CClockAtomic::OutputFn(CDEVSimMessage &_msg)
{
	//RoadInfo 신호를 주어서 마지막 collector를 initiate시킨다. 
	RoadInfo* emptyRoad = new RoadInfo;		
	_msg.SetPortValue(outport_last_coll.c_str(), new CRoadInfoMsg(*emptyRoad));

	char buf[100];
	sprintf_s(buf, 100, "\n\n\n\n\n%d round Initiated.", m_round++); // 새로운 라운드 시작시켰다는걸 로그에 출력해주고 round 증가시킴
	LOG((CAT_SHOW, LOG_INFO, buf));
	
	return true;
}

TimeType CClockAtomic::TimeAdvanceFn()
{
	return 1;
}
#include "Decision.h"

std::string CDecision::inport = "in";
std::string CDecision::outport = "out";


/*
Decision은 inport도 하나, outport도 하나인 간단한 구조이다.
Collector로부터 RoadInfo를 받아 OrderInfo로 뱉어준다.

현재의 Logic으로는 앞칸을 살펴보기 전에 Match가 우선된다.
여러가지 Logic을 생각해 보는 것이 좋겠다.
Logic은 수정할 수 있는 부분이 많은데, 만약 수정할 부분이 있다면 Decision만 수정하면 된다.

최창범 교수님이 짜 주신 구조가 얼마나 대단한지 알 수 있다. 교수님 짱짱맨 >_<
*/
CDecision::CDecision()
{
	SetName("Decision");
	AddInPorts(1, inport.c_str());
	AddOutPorts(1, outport.c_str());

	cur_road_info = new RoadInfo;
	m_state = WAIT;
}

CDecision::~CDecision()
{
}

bool CDecision::ExtTransFn(const CDEVSimMessage & _msg)
{
	if (inport.compare(_msg.GetPort()) == 0) 
	{
		CRoadInfoMsg* road_status = (CRoadInfoMsg*)_msg.GetArg(0);
		(*cur_road_info) = road_status->data; // 받은 RoadInfo를 꺼내 (*cur_road_info)에 저장 
		m_state = SEND;
	}
	return true;
}

bool CDecision::IntTransFn()
{
	if (m_state == SEND)
		m_state = WAIT;
	return true;
}


//#define _GO 1;
//#define _BLOCKED 2;
//#define _MATCH 3;


bool CDecision::OutputFn(CDEVSimMessage & _msg)
{
	OrderInfo* orderInfo = new OrderInfo();

	// MATCH : 우선 택시랑 human의 id가 같은 경우 match부터 시킨다.
	if ((*cur_road_info).ThisHuman.agent_id == (*cur_road_info).ThisTaxi.agent_id)
	{
		if ((*cur_road_info).ThisHuman.agent_id != -1) // 빈칸끼리 매칭이 되는 경우를 방지
		{
			//몇번 agent들끼리 매칭이 되었는지 로그로 출력
			char buf[100];
			sprintf_s(buf, 100, "%d pair matched!", (*cur_road_info).ThisHuman.agent_id); 
			LOG((CAT_SHOW, LOG_INFO, buf));

			//가지고 있는 orderInfo를 바꿔줌
			orderInfo->human_order = _MATCH;
			orderInfo->taxi_order = _MATCH;
		}
		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);
		return true;
	}

	// MATCH가 아닌경우
	else
	{
		if ((*cur_road_info).ThisHuman.agent_id != -1)
		{
			if ((*cur_road_info).NextHuman.agent_id == -1)
			{
				orderInfo->human_order = _GO;
			}
			else
			{
				orderInfo->human_order = _BLOCKED;
			}
		}

		if ((*cur_road_info).ThisTaxi.agent_id != -1)
		{
			if ((*cur_road_info).NextTaxi.agent_id == -1)
			{
				orderInfo->taxi_order = _GO;
			}
			else
			{
				orderInfo->taxi_order = _BLOCKED;
			}
		}
		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);

	}
	return true;
}

TimeType CDecision::TimeAdvanceFn()
{
	if (m_state == SEND)
		return 0;
	else return Infinity;
}
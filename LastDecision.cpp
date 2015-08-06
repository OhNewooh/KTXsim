#include "LastDecision.h"

std::string CLastDecision::inport = "in";
std::string CLastDecision::outport = "out";

// Decision과 구조가 같기 때문에, 더 이상의 자세한 설명은 생략하고싶다.

CLastDecision::CLastDecision()
{
	SetName("LastDecision");
	AddInPorts(1, inport.c_str());
	AddOutPorts(1, outport.c_str());

	cur_road_info = new RoadInfo;
	m_state = WAIT;
}


CLastDecision::~CLastDecision()
{
}

bool CLastDecision::ExtTransFn(const CDEVSimMessage & _msg)
{
	if (inport.compare(_msg.GetPort()) == 0)
	{
		CRoadInfoMsg* road_status = (CRoadInfoMsg*)_msg.GetArg(0);
		(*cur_road_info) = road_status->data;
		m_state = SEND;
	}
	return true;
}

bool CLastDecision::IntTransFn()
{
	if (m_state == SEND)
		m_state = WAIT;
	return true;
}


//#define _GO 1;
//#define _BLOCKED 2;
//#define _MATCH 3;


bool CLastDecision::OutputFn(CDEVSimMessage & _msg)
{
	OrderInfo* orderInfo = new OrderInfo();

	//택시에 사람이 이미 타 있는 경우는 무조건 택시가 나간다.
	if ((*cur_road_info).ThisTaxi.is_taxi_filled == true)
	{
		orderInfo->taxi_order = _GO;

		char buf[100];
		sprintf_s(buf, 100, "pair_id %d going out now. 손님태운 택시가 나갑니다 추카추카 OnYu생일도 추카추카", (*cur_road_info).ThisTaxi.agent_id);
		LOG((CAT_TEST, LOG_INFO, buf));

		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);
	}

	// MATCH : 택시랑 human의 id가 같음
	else if ((*cur_road_info).ThisHuman.agent_id == (*cur_road_info).ThisTaxi.agent_id)
	{
		if ((*cur_road_info).ThisHuman.agent_id != -1)//빈칸끼리 매치되지 말라
		{
			//로그를 자꾸 찍고 있다. 지워도 상관 없다. 프로그램 수행 결과를 자기가 보기 편한대로 로그를 찍어보시길
			char buf[100];
			sprintf_s(buf, 100, "%d pair matched in the last cell", (*cur_road_info).ThisHuman.agent_id);
			LOG((CAT_TEST, LOG_INFO, buf));
			
			orderInfo->human_order = _MATCH;
			orderInfo->taxi_order = _MATCH;

		}
		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);
		return true;
	}

	else // 마지막 칸에서는 매치가 아니라면 막힌 것이다. 
	{
		orderInfo->human_order = _BLOCKED;
		orderInfo->taxi_order = _BLOCKED;

		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);
	}
// 다양한 가능성을 생각해보자면 손님태운 택시가 나가는 동시에 사람은 막혀있는 케이스도 있을 수도 있다.
//	하지만 우리 로직에서는 그런 일이 발생하지 않아 우선은 고려하지 않았다.
// 로직을 바꿀 경우 Last decision에서 발생할수 있는 다른 케이스들도 고려해줘야 할것이다.

	// 구현보다 logic이 더 어려운것 같다. 다양한 로직을 생각해주세요.
	return true;
}

TimeType CLastDecision::TimeAdvanceFn()
{
	if (m_state == SEND)
		return 0;
	else return Infinity;
}

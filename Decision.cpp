#include "Decision.h"

std::string CDecision::inport = "in";
std::string CDecision::outport = "out";


/*
Decision�� inport�� �ϳ�, outport�� �ϳ��� ������ �����̴�.
Collector�κ��� RoadInfo�� �޾� OrderInfo�� ����ش�.

������ Logic���δ� ��ĭ�� ���캸�� ���� Match�� �켱�ȴ�.
�������� Logic�� ������ ���� ���� ���ڴ�.
Logic�� ������ �� �ִ� �κ��� ������, ���� ������ �κ��� �ִٸ� Decision�� �����ϸ� �ȴ�.

��â�� �������� ¥ �ֽ� ������ �󸶳� ������� �� �� �ִ�. ������ ¯¯�� >_<
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
		(*cur_road_info) = road_status->data; // ���� RoadInfo�� ���� (*cur_road_info)�� ���� 
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

	// MATCH : �켱 �ýö� human�� id�� ���� ��� match���� ��Ų��.
	if ((*cur_road_info).ThisHuman.agent_id == (*cur_road_info).ThisTaxi.agent_id)
	{
		if ((*cur_road_info).ThisHuman.agent_id != -1) // ��ĭ���� ��Ī�� �Ǵ� ��츦 ����
		{
			//��� agent�鳢�� ��Ī�� �Ǿ����� �α׷� ���
			char buf[100];
			sprintf_s(buf, 100, "%d pair matched!", (*cur_road_info).ThisHuman.agent_id); 
			LOG((CAT_SHOW, LOG_INFO, buf));

			//������ �ִ� orderInfo�� �ٲ���
			orderInfo->human_order = _MATCH;
			orderInfo->taxi_order = _MATCH;
		}
		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);
		return true;
	}

	// MATCH�� �ƴѰ��
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
#include "LastDecision.h"

std::string CLastDecision::inport = "in";
std::string CLastDecision::outport = "out";

// Decision�� ������ ���� ������, �� �̻��� �ڼ��� ������ �����ϰ�ʹ�.

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

	//�ýÿ� ����� �̹� Ÿ �ִ� ���� ������ �ýð� ������.
	if ((*cur_road_info).ThisTaxi.is_taxi_filled == true)
	{
		orderInfo->taxi_order = _GO;

		char buf[100];
		sprintf_s(buf, 100, "pair_id %d going out now. �մ��¿� �ýð� �����ϴ� ��ī��ī OnYu���ϵ� ��ī��ī", (*cur_road_info).ThisTaxi.agent_id);
		LOG((CAT_TEST, LOG_INFO, buf));

		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);
	}

	// MATCH : �ýö� human�� id�� ����
	else if ((*cur_road_info).ThisHuman.agent_id == (*cur_road_info).ThisTaxi.agent_id)
	{
		if ((*cur_road_info).ThisHuman.agent_id != -1)//��ĭ���� ��ġ���� ����
		{
			//�α׸� �ڲ� ��� �ִ�. ������ ��� ����. ���α׷� ���� ����� �ڱⰡ ���� ���Ѵ�� �α׸� ���ñ�
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

	else // ������ ĭ������ ��ġ�� �ƴ϶�� ���� ���̴�. 
	{
		orderInfo->human_order = _BLOCKED;
		orderInfo->taxi_order = _BLOCKED;

		COrderInfoMsg* orderCommand = new COrderInfoMsg(*orderInfo);
		_msg.SetPortValue(outport.c_str(), orderCommand);
	}
// �پ��� ���ɼ��� �����غ��ڸ� �մ��¿� �ýð� ������ ���ÿ� ����� �����ִ� ���̽��� ���� ���� �ִ�.
//	������ �츮 ���������� �׷� ���� �߻����� �ʾ� �켱�� ������� �ʾҴ�.
// ������ �ٲ� ��� Last decision���� �߻��Ҽ� �ִ� �ٸ� ���̽��鵵 �������� �Ұ��̴�.

	// �������� logic�� �� ������ ����. �پ��� ������ �������ּ���.
	return true;
}

TimeType CLastDecision::TimeAdvanceFn()
{
	if (m_state == SEND)
		return 0;
	else return Infinity;
}

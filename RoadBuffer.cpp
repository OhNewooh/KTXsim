#include "RoadBuffer.h"
#include <assert.h> // assert(����)�� �����ϸ� ����� ��忡�� ���� ���� ���� ����Ǹ� �� ��� ���α׷��� ���߰� �ȴ�.

std::string CRoadBuffer::inport_obj = "inport_obj";
std::string CRoadBuffer::outport_obj = "outport_obj";
std::string CRoadBuffer::inport_coll = "inport_coll";
std::string CRoadBuffer::outport_coll = "outport_coll";


/*
RoadBuffer���� REPORT, SEND, WAIT ���°� �ִ�.
��� �Ǵ� �ý� object�� ������ REPORT ���·� ���� (�ܺ�õ��) 0�ʰ� ���� �� ���� ���¸� Collector���� �����ϰ� WAIT���� ����.(����õ��)
Decision���� ���� order�� ������ SEND ���·� ����.(�ܺ�õ��)
SEND ���¿����� �� ��ɿ� ���� �����̰�(_GO, _MATCH ���� �����ϴ� ��쿣 ���� ���°� �ٲ�Ƿ�) REPORT ���·� ���� �ٲ� ���� ��Ȳ�� �����ϰ� WAIT���� ����.(����õ��)

��, agent object�� ������ REPORT ���·� ���� ������ �� WAIT�� ����,
����� ������ SEND ���·� ���� ����� �����ϰ� REPORT�� ���� �����ϰ� WAIT�� ����.

��� : REPORT->WAIT /  SEND->REPORT->WAIT ������� �����δ�.

*/
CRoadBuffer::CRoadBuffer(int road_id, int road_type)
	: m_road_id(road_id), m_road_type(road_type)
{
	char buf[1000];
	
	if (road_type = HUMAN)
	{
		sprintf_s(buf, 1000, "Human Road[%d]", road_id);
	}
	else if (road_type = TAXI)
	{
		sprintf_s(buf, 1000, "Taxi Road[%d]", road_id);
	}
	else
	{
		sprintf_s(buf, 1000, "Unknown Road[%d]", road_id);
	}

	SetName(buf);

	AddInPorts(1, inport_obj.c_str());
	AddOutPorts(1, outport_obj.c_str());
	AddInPorts(1, inport_coll.c_str());
	AddOutPorts(1, outport_coll.c_str());

	// 2015.08.03 added by cbchoi
	m_human_list.push_back(HumanInfo());
	m_taxi_list.push_back(TaxiInfo());
	m_state = WAIT;
}


CRoadBuffer::~CRoadBuffer()
{
}

bool CRoadBuffer::ExtTransFn(const CDEVSimMessage &_msg)
{
	if (_msg.GetPort().compare(inport_obj.c_str()) == 0)
	{
		if (m_road_type == HUMAN)
		{
			CHumanInfoMsg* received_msg = ((CHumanInfoMsg*)_msg.GetArg(0));
			char buf[100];
			sprintf_s(buf, 100, "Human Road[%d] received Human id %d.", m_road_id, received_msg->data.agent_id);
			LOG((CAT_ALL, LOG_INFO, buf));

			m_human_list[0] = received_msg->data; // modified by cbchoi 2015.08.03 // always m_human_list contains only one element

		}
		else if (m_road_type == TAXI)
		{
			CTaxiInfoMsg* received_msg = ((CTaxiInfoMsg*)_msg.GetArg(0));
			char buf[100];
			sprintf_s(buf, 100, "Taxi Road[%d] received Taxi number %d.", m_road_id, received_msg->data.agent_id);
			LOG((CAT_SHOW, LOG_INFO, buf));
			
			m_taxi_list[0] = received_msg->data; // modified by cbchoi 2015.08.03 // always m_taxi_list contains only one element
		}

		if (m_state != SEND)	// obj�� coll ��Ʈ���� ���ÿ� msg�� ������ ��, send ���¸� �ǳʶٰ� report�� ���°��� �����ϱ� ����
		{
			m_state = REPORT;
		}
	}

	else if (_msg.GetPort().compare(inport_coll.c_str()) == 0) // collector���� Ŀ�ǵ带 �޾��� ���
	{
		if (m_road_type == HUMAN)
		{
			CCellOrderMsg* received_msg = ((CCellOrderMsg*)_msg.GetArg(0));
			command = received_msg->data; // ���� �޽������� ����� ������ command�� �ִ´�.

			if (received_msg->data.cellOrder != 0)
			{
				char buf[100];
				sprintf_s(buf, 100, "Human Road[%d] received command %d.", m_road_id, received_msg->data.cellOrder);
				LOG((CAT_SHOW, LOG_INFO, buf));
			}

		}
		else if (m_road_type == TAXI) 
		{
			CCellOrderMsg* received_msg = ((CCellOrderMsg*)_msg.GetArg(0));
			command = received_msg->data;

			if (received_msg->data.cellOrder != 0)
			{
				char buf[100];
				sprintf_s(buf, 100, "Taxi Road[%d] received command %d.", m_road_id, received_msg->data.cellOrder);
				LOG((CAT_SHOW, LOG_INFO, buf));
			}
		}
		m_state = SEND; // Ŀ�ǵ带 �����ϴ� SEND ���� ����.
	}
	else
	{
		LOG((CAT_ALL, LOG_ERROR, "Unexpected msg"));
		assert(0);
	}
	return true;
}

bool CRoadBuffer::OutputFn(CDEVSimMessage &_msg)
{
	if (m_state == REPORT) // collector�� ������Ʈ �޽��� (�ڱ� ���� ���� ������Ʈ�� �ִ��� �����Ѵ�)
	{
		if (m_road_type == HUMAN)
		{
			_msg.SetPortValue(outport_coll.c_str(), new CHumanInfoMsg(m_human_list[0]));
		}
		else if (m_road_type == TAXI)
		{
			_msg.SetPortValue(outport_coll.c_str(), new CTaxiInfoMsg(m_taxi_list[0]));
		}
	}
	else // SEND : command�� ���� (���� ���� ������Ʈ �޽���)
	{
		if (m_road_type == HUMAN)
		{
			if (command.cellOrder == _GO)
			{
				_msg.SetPortValue(outport_obj.c_str(), new CHumanInfoMsg(m_human_list[0]));
			}
		}
		else if (m_road_type == TAXI)
		{
			if (command.cellOrder == _GO)
			{
				_msg.SetPortValue(outport_obj.c_str(), new CTaxiInfoMsg(m_taxi_list[0])); 
			}
		}
	}
	return true;
}
bool CRoadBuffer::IntTransFn()
{
	//��� SEND ���¿����� GO ���� ����� �޾��� ��� OutputFn�� ���� ���� ���� object�� �Ѱ��־���.
	//���� IntTransFn �ȿ����� ���� �ִ� object�� �����. -1 id�� �־������ν� �� ĭ�� ���� ǥ���Ѵ�.
	if (m_state == SEND)
	{
		if (m_road_type == HUMAN)
		{
			if (command.cellOrder == _GO || command.cellOrder == _MATCH)//_GO or _MATCH
			{
				m_human_list[0].agent_id = -1; // ���� ��� �� -1�� �־��ش�.
			}

		}
		else if(m_road_type == TAXI)
		{
			if (command.cellOrder == _GO)
			{
				m_taxi_list[0].agent_id = -1;
				m_taxi_list[0].is_taxi_filled = false;
			}

			else if (command.cellOrder == _MATCH)//_MATCH
			{
				m_taxi_list[0].is_taxi_filled = true;
			}
		}
		m_state = REPORT;
	}
	else if (m_state == REPORT)
		m_state = WAIT;
	return true;
}


TimeType CRoadBuffer::TimeAdvanceFn()
{
	if (m_state == WAIT)
		return Infinity;
	else
		return 0;
}
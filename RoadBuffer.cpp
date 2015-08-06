#include "RoadBuffer.h"
#include <assert.h> // assert(조건)을 선언하면 디버깅 모드에서 조건 밖의 값이 선언되면 그 즉시 프로그램이 멈추게 된다.

std::string CRoadBuffer::inport_obj = "inport_obj";
std::string CRoadBuffer::outport_obj = "outport_obj";
std::string CRoadBuffer::inport_coll = "inport_coll";
std::string CRoadBuffer::outport_coll = "outport_coll";


/*
RoadBuffer에는 REPORT, SEND, WAIT 상태가 있다.
사람 또는 택시 object를 받으면 REPORT 상태로 가서 (외부천이) 0초가 지난 후 길의 상태를 Collector에게 보고하고 WAIT으로 간다.(내부천이)
Decision에서 보낸 order를 받으면 SEND 상태로 간다.(외부천이)
SEND 상태에서는 그 명령에 따라 움직이고(_GO, _MATCH 등을 수행하는 경우엔 길의 상태가 바뀌므로) REPORT 상태로 가서 바뀐 길의 상황을 보고하고 WAIT으로 간다.(내부천이)

즉, agent object를 받으면 REPORT 상태로 가서 보고한 후 WAIT에 가고,
명령을 받으면 SEND 상태로 가서 명령을 수행하고 REPORT에 가서 보고하고 WAIT로 간다.

요약 : REPORT->WAIT /  SEND->REPORT->WAIT 순서대로 움직인다.

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

		if (m_state != SEND)	// obj와 coll 포트에서 동시에 msg가 들어왔을 때, send 상태를 건너뛰고 report로 가는것을 방지하기 위해
		{
			m_state = REPORT;
		}
	}

	else if (_msg.GetPort().compare(inport_coll.c_str()) == 0) // collector에서 커맨드를 받았을 경우
	{
		if (m_road_type == HUMAN)
		{
			CCellOrderMsg* received_msg = ((CCellOrderMsg*)_msg.GetArg(0));
			command = received_msg->data; // 받은 메시지에서 명령을 꺼내서 command에 넣는다.

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
		m_state = SEND; // 커맨드를 실행하는 SEND 모드로 간다.
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
	if (m_state == REPORT) // collector로 오브젝트 메시지 (자기 셀에 무슨 오브젝트가 있는지 보고한다)
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
	else // SEND : command를 수행 (다음 셀로 오브젝트 메시지)
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
	//방금 SEND 상태에서는 GO 등의 명령을 받았을 경우 OutputFn을 통해 다음 셀로 object를 넘겨주었다.
	//이제 IntTransFn 안에서는 갖고 있던 object를 지운다. -1 id를 넣어줌으로써 빈 칸인 것을 표현한다.
	if (m_state == SEND)
	{
		if (m_road_type == HUMAN)
		{
			if (command.cellOrder == _GO || command.cellOrder == _MATCH)//_GO or _MATCH
			{
				m_human_list[0].agent_id = -1; // 셀을 비울 때 -1을 넣어준다.
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
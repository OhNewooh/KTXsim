#include "Receiver.h"
#include "message_def.h"

std::string CReceiver::inport_hum = "inport_hum";
std::string CReceiver::inport_taxi = "inport_taxi";
std::string CReceiver::inport_next_collector = "in_next_coll";

std::string CReceiver::outport_decision = "out_decision";

CReceiver::CReceiver(int rec_id)
	:m_rec_id(rec_id)
{
	SetName("Receiver"); // receiver 이름에 번호 안넣었네..zz
	AddInPorts(1, inport_hum.c_str());
	AddInPorts(1, inport_taxi.c_str());
	AddInPorts(1, inport_next_collector.c_str());

	AddOutPorts(1, outport_decision.c_str());

	m_state = WAIT;
	cur_road_info = new RoadInfo();
}


CReceiver::~CReceiver()
{
}


bool CReceiver::ExtTransFn(const CDEVSimMessage & _msg)
{
	if (inport_hum.compare(_msg.GetPort()) == 0)
	{
		CHumanInfoMsg* hum_info_msg = (CHumanInfoMsg*)_msg.GetArg(0);
		(*cur_road_info).ThisHuman = hum_info_msg->data;
	}
	else if (inport_taxi.compare(_msg.GetPort()) == 0)
	{
		CTaxiInfoMsg* taxi_info_msg = (CTaxiInfoMsg*)_msg.GetArg(0);
		(*cur_road_info).ThisTaxi = taxi_info_msg->data;
	}
	else if (inport_next_collector.compare(_msg.GetPort()) == 0)
	{
		CRoadInfoMsg* next_road = (CRoadInfoMsg*)_msg.GetArg(0);
		(*cur_road_info).NextHuman = next_road->data.ThisHuman;
		(*cur_road_info).NextTaxi = next_road->data.ThisTaxi;
		//CAtomic::Continue();
		m_state = SEND;
	}
	return true;
}

bool CReceiver::IntTransFn()
{
	if (m_state == SEND)
		m_state = WAIT;
	return true;
}

bool CReceiver::OutputFn(CDEVSimMessage & _msg)
{
	_msg.SetPortValue(outport_decision.c_str(), new CRoadInfoMsg(*cur_road_info));
	return true;
}

TimeType CReceiver::TimeAdvanceFn()
{
	if (m_state == SEND)
		return 0;
	else
		return Infinity;
}


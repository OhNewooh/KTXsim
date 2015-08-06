#include "Collector.h"
#include "Sender.h"
#include "Receiver.h"

std::string CCollector::inport_hum = "inport_hum";
std::string CCollector::inport_taxi = "inport_taxi";
std::string CCollector::inport_next_collector = "in_next_coll";
std::string CCollector::inport_decision = "in_decision";

std::string CCollector::outport_hum = "outport_hum";
std::string CCollector::outport_taxi = "outport_taxi";
std::string CCollector::outport_prev_collector = "out_prev_coll";
std::string CCollector::outport_decision = "out_decision";

CCollector::CCollector(int coll_id)
	:m_coll_id(coll_id)
{
	SetName("Collector");
//add ports
	AddInPorts(1, inport_hum.c_str());
	AddInPorts(1, inport_taxi.c_str());
	AddInPorts(1, inport_next_collector.c_str());
	AddInPorts(1, inport_decision.c_str());

	AddOutPorts(1, outport_hum.c_str());
	AddOutPorts(1, outport_taxi.c_str());
	AddOutPorts(1, outport_prev_collector.c_str());
	AddOutPorts(1, outport_decision.c_str());

//add components
	CModel* receiver = new CReceiver(m_coll_id);
	CModel* sender = new CSender(m_coll_id);

	AddComponent(1, receiver);
	AddComponent(1, sender);


//add couplings
	AddCoupling(this, inport_hum.c_str(), receiver, CReceiver::inport_hum.c_str());
	AddCoupling(this, inport_taxi.c_str(), receiver, CReceiver::inport_taxi.c_str());
	AddCoupling(this, inport_next_collector.c_str(), receiver, CReceiver::inport_next_collector.c_str());
	AddCoupling(this, inport_decision.c_str(), sender, CSender::inport_dec.c_str());

	AddCoupling(sender, CSender::outport_hum_road.c_str(), this, outport_hum.c_str());
	AddCoupling(sender, CSender::outport_taxi_road.c_str(), this, outport_taxi.c_str());
	AddCoupling(sender, CSender::outport_prev_collector.c_str(), this, outport_prev_collector.c_str());
	AddCoupling(receiver, CReceiver::outport_decision.c_str(), this, outport_decision.c_str());
}
CCollector::~CCollector()
{
}

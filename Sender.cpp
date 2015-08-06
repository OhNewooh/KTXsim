#include "Sender.h"

std::string CSender::inport_dec = "in_desision";
std::string CSender::outport_hum_road = "out_hum_road";
std::string CSender::outport_taxi_road = "out_taxi_road";
std::string CSender::outport_prev_collector = "out_prev_collector";

CSender::CSender(int sender_id)
	:m_sender_id(sender_id)
{
	SetName("Sender");

	AddInPorts(1, inport_dec.c_str());
	AddOutPorts(1, outport_hum_road.c_str());
	AddOutPorts(1, outport_taxi_road.c_str());
	AddOutPorts(1, outport_prev_collector.c_str());

	cur_road_info = new RoadInfo;
	m_state = WAIT;
	humanOrder = new CellOrder;
	taxiOrder = new CellOrder;
}


CSender::~CSender()
{
}


bool CSender::ExtTransFn(const CDEVSimMessage & _msg)
{

	if (inport_dec.compare(_msg.GetPort()) == 0)
	{
		//order�� ��ü�ؼ� �޸� �ε�� �ý� �ε�� ������ �޽����� ���� ���� ����� �ش�.
		COrderInfoMsg* orderMsg = (COrderInfoMsg*)_msg.GetArg(0);
		humanOrder->cellOrder = orderMsg->data.human_order;
		taxiOrder->cellOrder = orderMsg->data.taxi_order;

		m_state = DECIDED;
	}
	return true;
}

bool CSender::IntTransFn()
{
	m_state = WAIT;
	return true;
}

bool CSender::OutputFn(CDEVSimMessage & _msg)
{
	if (m_state == DECIDED)
	{
		//////////////////////////////////////////////////////////////////////////
		//decision���� �޽����� ���� ����, human road�� taxi road�� �޽����� ������
		//��Ī �Ǵ� ������ �Ѵ�.
		_msg.SetPortValue(outport_hum_road.c_str(), new CCellOrderMsg(*humanOrder));
		_msg.SetPortValue(outport_taxi_road.c_str(), new CCellOrderMsg(*taxiOrder));

		if (humanOrder->cellOrder == 1 || humanOrder->cellOrder == 3)
			(*cur_road_info).ThisHuman.agent_id = -1;

		if (taxiOrder->cellOrder == 1)
			(*cur_road_info).ThisTaxi.agent_id = -1;

		//���� �ݷ��ͷε� �޽����� ������.
		_msg.SetPortValue(outport_prev_collector.c_str(), new CRoadInfoMsg(*cur_road_info));
	}
	return true;
}

TimeType CSender::TimeAdvanceFn()
{
	if (m_state == WAIT)
		return Infinity;
	else
		return 0;
}


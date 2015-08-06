#pragma once
#include "DEVSim.h"


/* 1. agent info : Human, Taxi�� �ִ�.
agent_id�� �����ڿ��� -1�� �ʱ�ȭ�Ǵµ�, -1�� �ƹ��� ���ٴ� ���� ��Ÿ����.
Taxi�� is_taxi_filled�� ���� �ýÿ� ����� ������ Ȯ���Ѵ�.
�ýð� �� ���� �翬�� ����� �� Ÿ�� �����Ƿ� �̰��� false�� �ʱ�ȭ�ȴ�.
*/

//1. agent info
class HumanInfo
{
public:
	HumanInfo()
	{
		agent_id = -1;
	}
public:
	int agent_id;
};

class TaxiInfo
{
public:
	TaxiInfo()
	{
		agent_id = -1;
		is_taxi_filled = false;
	}

public:
	int agent_id;
	bool is_taxi_filled;

};

/* 2. envelope : �޽����� �������� CDEVSimMessage Ŭ������ ��ӹ޾ƾ߸� �Ѵ�.
			 ������ �޽��� Ÿ���� ��� �ִ� ������Ʈ�� �̸��� ��Ÿ����.
			 ���� ��� HumanInfo�� ���� �� �ִ� �޽��� ������ �̸���
			 CHumanInfoMsg�̴�.
*/
class CHumanInfoMsg : public CDEVSimMessage
{
public:
	CHumanInfoMsg(HumanInfo _data) : data(_data){}
	// �� �޽����� data��� HumanInfo�� ���� �ִ�. �Ķ���ͷ� _data�� �޾Ƽ� �ʱ�ȭ��Ų��.
	//�� data = _data; �� ����.
	~CHumanInfoMsg(){}

public:
	HumanInfo data;
};

class CTaxiInfoMsg : public CDEVSimMessage
{
public:
	CTaxiInfoMsg(TaxiInfo _data) : data(_data){}
	~CTaxiInfoMsg(){}

public:
	TaxiInfo data;
};



/************************************************************************
   RoadInfo�� ���� ��� �� �濡 ����ִ� ���/�ý� ��ü�� ������ ��� �ִ�.
   �׷��� RoadInfo�� HumanInfo, TaxiInfo�� ��� �ִ�.

   ������ �ݷ��ʹ� Road�� ��Ȳ�� �˾ƾ� �ϹǷ� RoadInfo�� �����ִ�.

   (Collector�� �����ϰ� �ִ� Receiver.h�� ����
   cur_road_info��� RoadInfo�� ������ ������ ������ ������ ���� �ִ�.)

   ���� ���� ������ ��Ƽ� ���� �ִٰ�
   next collecter���� ���� ���� ������ �˷��ָ�
   �� �������� NextHuman, NextTaxi�� ��Ƽ� Decision���� ������.

   �׷� RoadInfo�� ��� �޽��� ������ �̸��� ����? CRoadInfoMsg�̴�.
   
   envelope���� �����ߵ�,
   �޽��� ������ C������Ʈ�̸�Msg ������ ���� CDEVSimMessage Ŭ������ ��ӹ޾ƾ� �Ѵ�.
************************************************************************/	
	

//RoadInfo�� ���� ��� �� �濡 ����ִ� ���/�ý� ��ü�� ������ ��� �ִ�.
class RoadInfo : public CDEVSObject
{
public:
	RoadInfo();
	~RoadInfo();

	HumanInfo ThisHuman;
	TaxiInfo ThisTaxi;
	HumanInfo NextHuman;
	TaxiInfo NextTaxi;

};

class CRoadInfoMsg : public CDEVSimMessage
{
public:
	CRoadInfoMsg(RoadInfo _data)
	{
		data = data;
	}

public:
	RoadInfo data;
};





#define _GO 1
#define _BLOCKED 2
#define _MATCH 3
//#define _MATCH_STOP 4;

/*
OrderInfo�� decision���� ��� ���� �����Ͽ� collector�� �Ѱ��� �� ����Ѵ�.
�ش� �÷��� ��� �ε����(��� ĭ, �ý� ĭ)�鿡 ���� ����� ����ִ�.
�׷��� human_order�� taxi order�� �� ����ִ�.
*/

class OrderInfo : public CDEVSObject
{
public:
	OrderInfo();
	~OrderInfo();

	int human_order;
	int taxi_order;

};
/*//////////////////////////////////////////////////////////////////////////

�� ��ɵ��� ���� 1, 2, 3���� �����ߴ�.
OrderInfo�� �����ڿ��� 0���� �ʱ�ȭ�ȴ�.

������ HumanInfo, TaxiInfo�� ��� ���Ͽ��� �����ڸ� �ٷ� ������ �־�����
OrderInfo�� �����ڴ� message_def.cpp ���Ͽ� �ִ�. alt + g�� ������ Ȯ���� ����.

�ƹ�ư, 0�̶�� ����� ������ �ƹ� �ϵ� ���� �ʴ´�.
//////////////////////////////////////////////////////////////////////////*/
class COrderInfoMsg : public CDEVSimMessage
{
public:
	COrderInfoMsg(OrderInfo _data)
	{
		data = _data;
	}

public:
	OrderInfo data;
};


//CellOrder�� collector���� ������ RoadBuffer�� ����� ���� �� ����Ѵ�.
//�� Cell�� ���� ����� ���� �ִ�. �� int �ϳ��� ���� �ִ�.

class CellOrder : public CDEVSObject
{
public:
	CellOrder();
	~CellOrder();

	int cellOrder;

};

class CCellOrderMsg : public CDEVSimMessage
{
public:
	CCellOrderMsg(CellOrder _data)
	{
		data = _data;
	}

public:
	CellOrder data;
};
#pragma once
#include "DEVSim.h"


/* 1. agent info : Human, Taxi가 있다.
agent_id가 생성자에서 -1로 초기화되는데, -1은 아무도 없다는 것을 나타낸다.
Taxi는 is_taxi_filled를 통해 택시에 사람이 탔는지 확인한다.
택시가 올 때는 당연히 사람이 안 타고 있으므로 이것은 false로 초기화된다.
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

/* 2. envelope : 메시지로 보내려면 CDEVSimMessage 클래스를 상속받아야만 한다.
			 각각의 메시지 타입은 담고 있는 오브젝트의 이름을 나타낸다.
			 예를 들어 HumanInfo를 담을 수 있는 메시지 봉투의 이름은
			 CHumanInfoMsg이다.
*/
class CHumanInfoMsg : public CDEVSimMessage
{
public:
	CHumanInfoMsg(HumanInfo _data) : data(_data){}
	// 이 메시지는 data라는 HumanInfo를 갖고 있다. 파라미터로 _data를 받아서 초기화시킨다.
	//즉 data = _data; 와 같다.
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
   RoadInfo는 현재 길과 앞 길에 들어있는 사람/택시 객체의 정보를 담고 있다.
   그래서 RoadInfo는 HumanInfo, TaxiInfo를 담고 있다.

   각각의 콜렉터는 Road의 상황을 알아야 하므로 RoadInfo를 갖고있다.

   (Collector가 포함하고 있는 Receiver.h를 보면
   cur_road_info라는 RoadInfo의 포인터 변수를 가지고 있음을 볼수 있다.)

   현재 길의 정보를 모아서 갖고 있다가
   next collecter에서 다음 길의 정보를 알려주면
   그 정보들을 NextHuman, NextTaxi에 모아서 Decision으로 보낸다.

   그럼 RoadInfo를 담는 메시지 봉투의 이름은 뭘까? CRoadInfoMsg이다.
   
   envelope에서 설명했듯,
   메시지 봉투는 C오브젝트이름Msg 식으로 만들어서 CDEVSimMessage 클래스를 상속받아야 한다.
************************************************************************/	
	

//RoadInfo는 현재 길과 앞 길에 들어있는 사람/택시 객체의 정보를 담고 있다.
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
OrderInfo는 decision에서 모든 것을 결정하여 collector로 넘겨줄 때 사용한다.
해당 컬럼의 모든 로드버퍼(사람 칸, 택시 칸)들에 대한 명령이 담겨있다.
그래서 human_order와 taxi order가 다 들어있다.
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

이 명령들은 각각 1, 2, 3으로 구분했다.
OrderInfo는 생성자에서 0으로 초기화된다.

위에서 HumanInfo, TaxiInfo는 헤더 파일에서 생성자를 바로 선언해 주었지만
OrderInfo의 생성자는 message_def.cpp 파일에 있다. alt + g를 눌러서 확인해 보삼.

아무튼, 0이라는 명령을 받으면 아무 일도 하지 않는다.
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


//CellOrder는 collector에서 각각의 RoadBuffer에 명령을 내릴 때 사용한다.
//한 Cell에 대한 명령을 갖고 있다. 즉 int 하나만 갖고 있다.

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
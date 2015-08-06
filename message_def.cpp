#pragma once
#include "message_def.h"



RoadInfo::RoadInfo()
{
}
RoadInfo::~RoadInfo()
{ }

//0은 아무 일도 하지 않는다. 즉 아무 명령도 내리지 않는 것이다.
OrderInfo::OrderInfo()
{
	human_order = 0;
	taxi_order = 0;
}
OrderInfo::~OrderInfo()
{ }


CellOrder::CellOrder()
{
	cellOrder = 0;
}
CellOrder::~CellOrder()
{ }


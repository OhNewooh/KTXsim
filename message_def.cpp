#pragma once
#include "message_def.h"



RoadInfo::RoadInfo()
{
}
RoadInfo::~RoadInfo()
{ }

//0�� �ƹ� �ϵ� ���� �ʴ´�. �� �ƹ� ��ɵ� ������ �ʴ� ���̴�.
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


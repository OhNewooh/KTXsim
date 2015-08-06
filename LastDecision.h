#pragma once
#include "DEVSim.h"
#include "message_def.h"
#include <assert.h>
#include "engine_def.h"

/*
Last Decision�� ��Ī�� �� �Ǹ� GO�� �� �� ���� ��Ģ�� �ٲ��� Decision�̴�. ������ decision�� �Ȱ���.

Last decision�� ���� �׳� decision�� ���� ���,
RoadInfo�� ��� �ִ� NextHuman, NextTaxi�� agent_id�� -1�� �ʱ�ȭ �ǹǷ�
���� ĭ�� ����ٰ� �����ϰ� Agent���� GO�� ���� ������ ����� ������.

*/
class CLastDecision : public CAtomic
{
public:
	CLastDecision();
	~CLastDecision();

	static std::string inport;
	static std::string outport;

public:
	virtual bool ExtTransFn(const CDEVSimMessage & _msg);
	virtual bool IntTransFn();
	virtual bool OutputFn(CDEVSimMessage & _msg);
	virtual TimeType TimeAdvanceFn();

	//status
public:
	enum SysState { SEND, WAIT };
	SysState m_state;

public:
	RoadInfo* cur_road_info;
};


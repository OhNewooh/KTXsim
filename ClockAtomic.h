#pragma once

#include "DEVSim.h"
#include "engine_def.h"
#include "message_def.h"

/*
이건 원래 테스트 코드로 아무 역할도 하지 않던 것을 마지막 collector에 신호를 줘서 일을 시작하게 만들었다.
클래스의 이름을 round initiater 등으로 바꾸고 싶지만 일단 혼동을 줄까봐 그대로 두었다.

collector는 다음 도로의 상황을 받아야 현재 도로의 상황을 decision에게 전달하기 때문에
얘가 마지막 collector에게 신호를 주면 마지막 칸부터 행동을 시작한다.

1초에 1번씩 신호를 주는 기능뿐이 없다.
몇번 진행했는지 보여주기 위해 int m_round를 하나 넣었다.
*/
class CClockAtomic : public CAtomic
{
public:
	CClockAtomic();
	~CClockAtomic();

public:
	virtual bool ExtTransFn(const CDEVSimMessage &);
	virtual bool IntTransFn();
	virtual bool OutputFn(CDEVSimMessage &);
	virtual TimeType TimeAdvanceFn();

public:
	static std::string outport_last_coll;

public:
	enum TState{ INIT };
	TState m_state;
	int m_round;

};
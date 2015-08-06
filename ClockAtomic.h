#pragma once

#include "DEVSim.h"
#include "engine_def.h"
#include "message_def.h"

/*
�̰� ���� �׽�Ʈ �ڵ�� �ƹ� ���ҵ� ���� �ʴ� ���� ������ collector�� ��ȣ�� �༭ ���� �����ϰ� �������.
Ŭ������ �̸��� round initiater ������ �ٲٰ� ������ �ϴ� ȥ���� �ٱ�� �״�� �ξ���.

collector�� ���� ������ ��Ȳ�� �޾ƾ� ���� ������ ��Ȳ�� decision���� �����ϱ� ������
�갡 ������ collector���� ��ȣ�� �ָ� ������ ĭ���� �ൿ�� �����Ѵ�.

1�ʿ� 1���� ��ȣ�� �ִ� ��ɻ��� ����.
��� �����ߴ��� �����ֱ� ���� int m_round�� �ϳ� �־���.
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
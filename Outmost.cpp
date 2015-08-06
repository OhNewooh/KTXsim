#include "Outmost.h"
#include "ClockAtomic.h "
#include "Generator.h"
#include "RoadBuffer.h"
#include "Collector.h"
#include "Decision.h"
#include "LastDecision.h"
//#include "Buffer.h"

COutmost::COutmost()
{
	SetName("Outmost");

	CModel* RoundInit = new CClockAtomic;
	AddComponent(1, RoundInit);


	CModel* pGen1 = new CGenerator("HumanGen", HUMAN, 2.0);
	CModel* pGen2 = new CGenerator("TaxiGen", TAXI, 1.0);

	AddComponent(1, pGen1);
	AddComponent(1, pGen2);


	std::vector<CModel*> pHumanRoad;
	std::vector<CModel*> pTaxiRoad;
	std::vector<CModel*> pCollector;
	std::vector<CModel*> pDecision;

	//Road[0]은 object를 벡터로 쌓는 버퍼 역할
	//Buffer.cpp의 주석 참고
//	pHumanRoad.push_back(new CBuffer(HUMAN));
//	pTaxiRoad.push_back(new CBuffer(TAXI));
	


	for (int i = 0; i < 5; i++)
	{
		pHumanRoad.push_back(new CRoadBuffer(i, HUMAN));
		pTaxiRoad.push_back(new CRoadBuffer(i, TAXI));
		pCollector.push_back(new CCollector(i));
		pDecision.push_back(new CDecision());
	}

	//Road[0]을 하나 붙였었으니까 얘네는 길 뒤에서 하나씩 빼주기
//	pHumanRoad.pop_back();
//	pTaxiRoad.pop_back(); 

	//Last decision 붙여주기
	pDecision.pop_back();
	pDecision.push_back(new CLastDecision());


	for (std::vector<CModel*>::iterator iter = pHumanRoad.begin(); iter != pHumanRoad.end(); iter++)
	{
		AddComponent(1, (*iter));
	}

	for (std::vector<CModel*>::iterator iter = pTaxiRoad.begin(); iter != pTaxiRoad.end(); iter++)
	{
		AddComponent(1, (*iter));
	}

	for (std::vector<CModel*>::iterator iter = pCollector.begin(); iter != pCollector.end(); iter++)
	{
		AddComponent(1, (*iter));
	}

	for (std::vector<CModel*>::iterator iter = pDecision.begin(); iter != pDecision.end(); iter++)
	{
		AddComponent(1, (*iter));
	}

	//gen과 첫번째 roadbuffer 연결
	AddCoupling(pGen1, CGenerator::outport.c_str(), pHumanRoad[0], CRoadBuffer::inport_obj.c_str());
	AddCoupling(pGen2, CGenerator::outport.c_str(), pTaxiRoad[0], CRoadBuffer::inport_obj.c_str());
//
//	AddCoupling(pHumanRoad[0], CBuffer::outport_obj.c_str(), pHumanRoad[1], CRoadBuffer::inport_obj.c_str());
//	AddCoupling(pTaxiRoad[0], CBuffer::outport_obj.c_str(), pTaxiRoad[1], CRoadBuffer::inport_obj.c_str());

	for (int i = 0; i < 4; i++)
	{
		//roadbuffer 사이들을 연결
		AddCoupling(pHumanRoad[i], CRoadBuffer::outport_obj.c_str(), pHumanRoad[i + 1], CRoadBuffer::inport_obj.c_str());
		AddCoupling(pTaxiRoad[i], CRoadBuffer::outport_obj.c_str(), pTaxiRoad[i + 1], CRoadBuffer::inport_obj.c_str());
	}

	for (int i = 0; i < 4; i++)
	{
		//collector 사이들을 연결
		AddCoupling(pCollector[4 - i], CCollector::outport_prev_collector.c_str(), pCollector[3 - i], CCollector::inport_next_collector.c_str());

		//collector와 decision을 연결
		AddCoupling(pCollector[i], CCollector::outport_decision.c_str(), pDecision[i], CDecision::inport.c_str());
		AddCoupling(pDecision[i], CDecision::outport.c_str(), pCollector[i], CCollector::inport_decision.c_str());
	}

	//마지막 collector와 decision 연결
	AddCoupling(pCollector[4], CCollector::outport_decision.c_str(), pDecision[4], CLastDecision::inport.c_str());
	AddCoupling(pDecision[4], CLastDecision::outport.c_str(), pCollector[4], CCollector::inport_decision.c_str());


	//
//	AddCoupling(pHumanRoad[0], CBuffer::outport_coll.c_str(), pCollector[0], CCollector::inport_hum.c_str());
//	AddCoupling(pTaxiRoad[0], CBuffer::outport_coll.c_str(), pCollector[0], CCollector::inport_taxi.c_str());
//	AddCoupling(pCollector[0], CCollector::outport_hum.c_str(), pHumanRoad[0], CBuffer::inport_coll.c_str());
//	AddCoupling(pCollector[0], CCollector::outport_taxi.c_str(), pTaxiRoad[0], CBuffer::inport_coll.c_str());

	//로드버퍼와 콜렉터 연결
	for (int i = 0; i < 5; i++)
	{
		AddCoupling(pHumanRoad[i], CRoadBuffer::outport_coll.c_str(), pCollector[i], CCollector::inport_hum.c_str());
		AddCoupling(pTaxiRoad[i], CRoadBuffer::outport_coll.c_str(), pCollector[i], CCollector::inport_taxi.c_str());
		AddCoupling(pCollector[i], CCollector::outport_hum.c_str(), pHumanRoad[i], CRoadBuffer::inport_coll.c_str());
		AddCoupling(pCollector[i], CCollector::outport_taxi.c_str(), pTaxiRoad[i], CRoadBuffer::inport_coll.c_str());

	}


	//마지막 collector에 Clock 연결
	AddCoupling(RoundInit, CClockAtomic::outport_last_coll.c_str(), pCollector[4], CCollector::inport_next_collector.c_str());



}


COutmost::~COutmost()
{
}
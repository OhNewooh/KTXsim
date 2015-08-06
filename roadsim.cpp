#include <iostream>
#include "DEVSim.h"
#include "Outmost.h"
#include "engine_def.h"
int main()
{
	CREATE_LOG_FILE(CAT_ALL, LOG_DEBUG, "test"); 
	//////////////////////////////////////////////////////////////////////////
	//카테고리와 레벨을 준 것임.


//	CREATE_LOG_STD(CAT_ALL, LOG_DEBUG); // 로그 표시

	// DEVSimInterface 설정
	CDEVSimInterface* pInterface = CDEVSimInterfaceDefaultFactory::create();
	SET_DEVSINTERFACE(pInterface);

	pInterface->SimOption.TimeMode = CSimOption::SECOND;
	pInterface->SimOption.bRealtime = true;
	pInterface->SimOption.bMultiThread = false;

	COutmost *model = new COutmost();

	pInterface->SetModel(model);

	pInterface->SimulationStart();

	//pInterface->SimOption.bMultiThread = false; 이므로
	// 다음 함수에서 종료시까지 Blocking
	pInterface->EngineStart();


	// 모델 삭제
	delete pInterface->GetModel();
	pInterface->SetModel(NULL);

	CDEVSimInterfaceDefaultFactory::destroy(pInterface);
	pInterface = NULL;

	return 0;
}
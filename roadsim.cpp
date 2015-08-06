#include <iostream>
#include "DEVSim.h"
#include "Outmost.h"
#include "engine_def.h"
int main()
{
	CREATE_LOG_FILE(CAT_ALL, LOG_DEBUG, "test"); 
	//////////////////////////////////////////////////////////////////////////
	//ī�װ��� ������ �� ����.


//	CREATE_LOG_STD(CAT_ALL, LOG_DEBUG); // �α� ǥ��

	// DEVSimInterface ����
	CDEVSimInterface* pInterface = CDEVSimInterfaceDefaultFactory::create();
	SET_DEVSINTERFACE(pInterface);

	pInterface->SimOption.TimeMode = CSimOption::SECOND;
	pInterface->SimOption.bRealtime = true;
	pInterface->SimOption.bMultiThread = false;

	COutmost *model = new COutmost();

	pInterface->SetModel(model);

	pInterface->SimulationStart();

	//pInterface->SimOption.bMultiThread = false; �̹Ƿ�
	// ���� �Լ����� ����ñ��� Blocking
	pInterface->EngineStart();


	// �� ����
	delete pInterface->GetModel();
	pInterface->SetModel(NULL);

	CDEVSimInterfaceDefaultFactory::destroy(pInterface);
	pInterface = NULL;

	return 0;
}
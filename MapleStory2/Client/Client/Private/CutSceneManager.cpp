#include "stdafx.h"
#include "..\Public\CutSceneManager.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "UIManager.h"
#include "GAS.h"
#include "ToolManager.h"

IMPLEMENT_SINGLETON(CCutSceneManager)

CCutSceneManager::CCutSceneManager()
{

}



void CCutSceneManager::Tick(_float fTimeDelta)
{

	if (nullptr == Get_MainCam())
		return;


	if (m_bEnter_Henesys_1)
		Enter_Henesys_1(fTimeDelta);

	if (m_bEnter_Henesys_2)
		Enter_Henesys_2(fTimeDelta);

	if (m_bEnter_Elenya)
		Enter_Elenya(fTimeDelta);

	if (m_bEnter_ElenyaEnter)
		Enter_ElenyaEnter(fTimeDelta);

	if (m_bEnter_DHenesys)
		Enter_DHenesys(fTimeDelta);

	if (m_bEnter_GASHenesys)
		Enter_GASHenesys(fTimeDelta);

	if (m_bEnter_InitGASHenesys)
		Enter_InitGASHenesys(fTimeDelta);

	if (m_bEnter_InitDHenesys)
		Enter_InitDHenesys(fTimeDelta);

	if (m_bEnter_BlackMage)
		Enter_BlackMage(fTimeDelta);

}

CCamera_Free* CCutSceneManager::Get_MainCam()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Free* pCam = (CCamera_Free*)CGameInstance::Get_Instance()->Get_ObjectPtr(LEVEL_STATIC, TEXT("Layer_Camera"), 0);
	
	Safe_Release(pGameInstance);

	return pCam;
}


void CCutSceneManager::SetCamPos(_float3 vPos)
{
	Get_MainCam()->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos);
}




void CCutSceneManager::Start_Enter_Henesys_1()
{
	if (m_bHenesys_1)
		return;
	m_bHenesys_1 = true;

	m_fHenesys_1_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
	SetCamPos(_float3(-20.f, 12.f, -16.f));
	Get_MainCam()->SetSpeed(3.f);
	m_bIsCutScene = true;
	m_bEnter_Henesys_1 = true;
	m_bMainUI = false;
}
void CCutSceneManager::Enter_Henesys_1(_float fTimeDelta)
{
	m_fHenesys_1_TimeAcc += fTimeDelta;

	if (1.f < m_fHenesys_1_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}


	Get_MainCam()->Get_Transform()->Go_Right(fTimeDelta);

	if (13.f < m_fHenesys_1_TimeAcc && !m_b1)
	{
		Get_MainCam()->Get_Transform()->RotationTwo(_float3(1.f, 0.f, 0.f), 30.f, _float3(0.f, 1.f, 0.f), -30.f);
		SetCamPos(_float3(20.f, 12.f, -30.f));
		m_b1 = true;
	}

	if (18.f < m_fHenesys_1_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_Henesys_1();
	}

}
void CCutSceneManager::End_Enter_Henesys_1()
{
	m_fHenesys_1_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);
	m_bIsCutScene = false;
	m_bEnter_Henesys_1 = false;

	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);
}



void CCutSceneManager::Start_Enter_Henesys_2()
{
	if (m_bHenesys_2)
		return;
	m_bHenesys_2 = true;

	m_fHenesys_2_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
	SetCamPos(_float3(-10.f, 12.f, -16.f));
	Get_MainCam()->SetSpeed(3.f);
	m_bIsCutScene = true;
	m_bEnter_Henesys_2 = true;
	m_bMainUI = false;
}
void CCutSceneManager::Enter_Henesys_2(_float fTimeDelta)
{
	m_fHenesys_2_TimeAcc += fTimeDelta;

	if (1.f < m_fHenesys_2_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}

	if(!m_b2_2)
		Get_MainCam()->Get_Transform()->Go_Right(fTimeDelta);
	else
		Get_MainCam()->Get_Transform()->Go_Up(fTimeDelta);



	if (13.f < m_fHenesys_2_TimeAcc && !m_b2_1)
	{
		Get_MainCam()->Get_Transform()->RotationTwo(_float3(1.f, 0.f, 0.f), 30.f, _float3(0.f, 1.f, 0.f), -30.f);
		SetCamPos(_float3(30.f, 12.f, -30.f));
		m_b2_1 = true;
	}

	if (18.f < m_fHenesys_2_TimeAcc && !m_b2_2)
	{
		Get_MainCam()->Get_Transform()->RotationTwo(_float3(1.f, 0.f, 0.f), 30.f, _float3(0.f, 1.f, 0.f), 30.f);
		SetCamPos(_float3(30.f, 15.f, -40.f));
		m_b2_2 = true;
	}



	if (23.f < m_fHenesys_2_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_Henesys_2();
	}

}
void CCutSceneManager::End_Enter_Henesys_2()
{
	m_fHenesys_2_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);
	m_bIsCutScene = false;
	m_bEnter_Henesys_2 = false;

	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);
}



void CCutSceneManager::Start_Enter_Elenya()
{
	if (m_bElenya)
		return;
	m_bElenya = true;

	m_fElenya_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
	SetCamPos(_float3(15.f, 0.f, -25.f));
	Get_MainCam()->SetSpeed(3.f);
	m_bIsCutScene = true;
	m_bEnter_Elenya = true;
	m_bMainUI = false;
}
void CCutSceneManager::Enter_Elenya(_float fTimeDelta)
{
	m_fElenya_TimeAcc += fTimeDelta;

	if (1.f < m_fElenya_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}

	if (!m_b3_1)
		Get_MainCam()->Get_Transform()->Go_Up(fTimeDelta);
	else
		Get_MainCam()->Get_Transform()->Go_Right(fTimeDelta);



	if (13.f < m_fElenya_TimeAcc && !m_b3_1)
	{
		Get_MainCam()->Get_Transform()->RotationTwo(_float3(1.f, 0.f, 0.f), 40.f, _float3(0.f, 1.f, 0.f), 0.f);
		SetCamPos(_float3(0.f, 50.f, -30.f));
		m_b3_1 = true;
	}



	if (20.f < m_fElenya_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_Elenya();
	}

}
void CCutSceneManager::End_Enter_Elenya()
{
	m_fElenya_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);
	m_bIsCutScene = false;
	m_bEnter_Elenya = false;

	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);
}





void CCutSceneManager::Start_Enter_ElenyaEnter()
{
	if (m_bElenyaEnter)
		return;
	m_bElenyaEnter = true;


	m_fElenyaEnter_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	Get_MainCam()->Get_Transform()->RotationTwo(_float3(1.f, 0.f, 0.f), 15.f, _float3(0.f, 1.f, 0.f), 90.f);
	SetCamPos(_float3(25.f, 7.f, -9.f));
	Get_MainCam()->SetSpeed(1.f);

	CUIManager::Get_Instance()->On_CutScreen();
	
	m_bEnter_ElenyaEnter = true;
	m_bIsCutScene = true;
	m_bMainUI = false;
}
void CCutSceneManager::Enter_ElenyaEnter(_float fTimeDelta)
{
	m_fElenyaEnter_TimeAcc += fTimeDelta;

	if (1.f < m_fElenyaEnter_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}

	if (!m_b4_1)
		Get_MainCam()->Get_Transform()->Go_Straight(fTimeDelta);



	if (7.5f < m_fElenyaEnter_TimeAcc && !m_b4_1)
	{
		m_b4_1 = true;
	}



	if (12.f < m_fElenyaEnter_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_ElenyaEnter();
	}
}
void CCutSceneManager::End_Enter_ElenyaEnter()
{
	m_fElenyaEnter_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);

	CUIManager::Get_Instance()->Off_CutScreen();

	m_bIsCutScene = false;
	m_bEnter_ElenyaEnter = false;

	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);
}





void CCutSceneManager::Start_Enter_GASHenesys()
{
	if (m_bGASHenesys)
		return;
	m_bGASHenesys = true;

	m_fGASHenesys_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	SetCamPos(_float3(7.8f, 11.5f, -9.f));
	Get_MainCam()->Get_Transform()->RotationTwo(_float3(0.f, 1.f, 0.f), 10.f, _float3(1.f, 0.f, 0.f), 30.f);
	Get_MainCam()->SetSpeed(3.f);

	CUIManager::Get_Instance()->On_CutScreen();

	m_bIsCutScene = true;
	m_bEnter_GASHenesys = true;
	m_bMainUI = false;
}
void CCutSceneManager::Enter_GASHenesys(_float fTimeDelta)
{
	m_fGASHenesys_TimeAcc += fTimeDelta;

	if (2.f < m_fGASHenesys_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}

	if (5.f < m_fGASHenesys_TimeAcc && !m_b6)
	{

		CGAS::GASDESC GASDesc;
		GASDesc.vPos = _float3(9.6f, 17.0f, -.3f);
		GASDesc.eState = CCreature::STATE_CUTSCEEN;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GAS"), LEVEL_HENESYS, TEXT("Layer_Monster"), &GASDesc)))
			return;

		m_b6 = true;
	}

	if (13.f < m_fGASHenesys_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_GASHenesys();
	}
}
void CCutSceneManager::End_Enter_GASHenesys()
{
	m_fGASHenesys_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);

	CUIManager::Get_Instance()->Off_CutScreen();

	m_bIsCutScene = false;
	m_bEnter_GASHenesys = false;
	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);
}





void CCutSceneManager::Start_Enter_InitGASHenesys()
{
	if (m_bInitGASHenesys)
		return;
	m_bInitGASHenesys = true;

	m_fInitGASHenesys_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	SetCamPos(_float3(0.f, 2.5f, 17.f));
	Get_MainCam()->Get_Transform()->Rotation(_float3(0.f, 1.f, 0.f), 0.f);
	Get_MainCam()->SetSpeed(0.1f);

	CUIManager::Get_Instance()->On_CutScreen();

	m_bIsCutScene = true;
	m_bEnter_InitGASHenesys = true;
	m_bMainUI = false;
}
void CCutSceneManager::Enter_InitGASHenesys(_float fTimeDelta)
{
	m_fInitGASHenesys_TimeAcc += fTimeDelta;

	if (1.f < m_fInitGASHenesys_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}
	

	if (!m_b7_1)
		Get_MainCam()->Get_Transform()->Go_Backward(fTimeDelta);	

	if (6.f < m_fInitGASHenesys_TimeAcc && !m_b7)
	{
		Get_MainCam()->SetSpeed(3.f);
		m_b7 = true;
	}

	if (12.f < m_fInitGASHenesys_TimeAcc && !m_b7_1)
	{
		m_b7_1 = true;
	}

	if (13.f < m_fInitGASHenesys_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_InitGASHenesys();
	}
}
void CCutSceneManager::End_Enter_InitGASHenesys()
{
	m_fInitGASHenesys_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);

	CUIManager::Get_Instance()->Off_CutScreen();
	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);

	m_bIsCutScene = false;
	m_bEnter_InitGASHenesys = false;
}







void CCutSceneManager::Start_Enter_InitDHenesys()
{

	if (m_bInitDHenesys)
		return;
	CGameInstance::Get_Instance()->StopAll();
	
	m_bInitDHenesys = true;

	m_fInitDHenesys_TimeAcc = 0.f;
	// Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	// Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
	// SetCamPos(_float3(-20.f, 12.f, -15.f));
	Get_MainCam()->SetSpeed(3.f);
	m_bIsCutScene = true;
	m_bEnter_InitDHenesys = true;
	m_bMainUI = false;

	CUIManager::Get_Instance()->On_CutScreen();

}
void CCutSceneManager::Enter_InitDHenesys(_float fTimeDelta)
{
	m_fInitDHenesys_TimeAcc += fTimeDelta;

	if (0.5f < m_fInitDHenesys_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}

	if (5.f < m_fInitDHenesys_TimeAcc && !m_b8)
	{
		Get_MainCam()->StartShake(0.4f, 0.05f, 15.f, _float3(1.f, 1.f, 1.f));
		CGameInstance::Get_Instance()->PlaySoundW(L"Shake.wav", 3, 1.f);
		m_b8 = true;
	}

	if (7.f < m_fInitDHenesys_TimeAcc && !m_b8_1)
	{
		CUIManager::Get_Instance()->On_DomiScreen();
		m_b8_1 = true;
	}
	
	if (19.f < m_fInitDHenesys_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_InitDHenesys();
		CToolManager::Get_Instance()->SetDestLevel(LEVEL_DHENESYS, _float3{17.f, 9.f, -4.f});
	}
}
void CCutSceneManager::End_Enter_InitDHenesys()
{
	m_fInitDHenesys_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);
	m_bIsCutScene = false;
	m_bEnter_InitDHenesys = false;

	CUIManager::Get_Instance()->Off_CutScreen();

	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);
}






void CCutSceneManager::Start_Enter_DHenesys()
{

	if (m_bDHenesys)
		return;
	m_bDHenesys = true;

	m_fDHenesys_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
	SetCamPos(_float3(-20.f, 12.f, -15.f));
	Get_MainCam()->SetSpeed(3.f);
	m_bIsCutScene = true;
	m_bEnter_DHenesys = true;
	m_bMainUI = false;

}
void CCutSceneManager::Enter_DHenesys(_float fTimeDelta)
{
	m_fDHenesys_TimeAcc += fTimeDelta;

	if (1.f < m_fDHenesys_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}

	Get_MainCam()->Get_Transform()->Go_Right(fTimeDelta);

	if (13.f < m_fDHenesys_TimeAcc && !m_b5_1)
	{
		Get_MainCam()->Get_Transform()->RotationTwo(_float3(1.f, 0.f, 0.f), 30.f, _float3(0.f, 1.f, 0.f), -30.f);
		SetCamPos(_float3(20.f, 12.f, -30.f));
		m_b5_1 = true;
	}

	if (18.f < m_fDHenesys_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		End_Enter_DHenesys();
	}
}
void CCutSceneManager::End_Enter_DHenesys()
{
	m_fDHenesys_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);
	m_bIsCutScene = false;
	m_bEnter_DHenesys = false;
	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);
}








void CCutSceneManager::Start_Enter_BlackMage()
{
	if (m_bBlackMage)
		return;
	m_bBlackMage = true;

	m_fBlackMage_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_CUTSCENE);
	Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 5.f);
	SetCamPos(_float3(0.f, 6.f, -30.f));
	Get_MainCam()->SetSpeed(1.f);
	m_bIsCutScene = true;
	m_bEnter_BlackMage = true;
	m_bMainUI = false;

	CUIManager::Get_Instance()->On_CutScreen();

}
void CCutSceneManager::Enter_BlackMage(_float fTimeDelta)
{
	m_fBlackMage_TimeAcc += fTimeDelta;

	if (1.f < m_fBlackMage_TimeAcc && !m_bMainUI)
	{
		CUIManager::Get_Instance()->Set_StartMove(true);
		CUIManager::Get_Instance()->Set_EndMove(false);
		m_bMainUI = true;
	}

	if(!m_b9)
		Get_MainCam()->Get_Transform()->Go_Straight(fTimeDelta);


	if (15.f < m_fBlackMage_TimeAcc && !m_b9)
	{
		m_b9 = true;
	}

	if (18.f < m_fBlackMage_TimeAcc)
	{
		Get_MainCam()->Get_Transform()->Rotation(_float3(1.f, 0.f, 0.f), 30.f);
		Get_MainCam()->Set_MaxPlayerCam();

		End_Enter_BlackMage();
	}
}
void CCutSceneManager::End_Enter_BlackMage()
{
	m_fBlackMage_TimeAcc = 0.f;
	Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);
	m_bIsCutScene = false;
	m_bEnter_BlackMage = false;
	CUIManager::Get_Instance()->Set_StartMove(false);
	CUIManager::Get_Instance()->Set_EndMove(true);

	CUIManager::Get_Instance()->Off_CutScreen();
}










void CCutSceneManager::Free()
{
	
}


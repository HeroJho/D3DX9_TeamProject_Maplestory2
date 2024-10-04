#include "stdafx.h"
#include "..\Public\Level_BlackMage.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "SkillInfo.h"
#include "SunCrossInfo.h"
#include "SolunaSlashInfo.h"
#include "SkillManager.h"
#include "UI.h"
#include "QuestManager.h"
#include "SpawnerManager.h"
#include "Maya.h"
#include "Level_Loading.h"
#include "Potal.h"
#include "CutSceneManager.h"
#include "UIManager.h"
#include "Sky.h"

CLevel_BlackMage::CLevel_BlackMage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_BlackMage::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	m_fCreateAcc = 0.f;

	CGameInstance::Get_Instance()->StopSound(0);
	CGameInstance::Get_Instance()->PlayBGM(L"DarkMage.mp3", 0.7f);

	CUIManager::Get_Instance()->End_Loading();

	CCutSceneManager::Get_Instance()->Start_Enter_BlackMage();

	return S_OK;
}

void CLevel_BlackMage::Tick(_float fTimeDelta)
{
	if (CQuestManager::Get_Instance()->Set_OrangeMushroom() >= 10)
		CQuestManager::Get_Instance()->QuestClear();
	__super::Tick(fTimeDelta);


	m_fCreateAcc += 1.f * fTimeDelta;
	if (m_fCreateAcc > 15.f)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardLaser"), LEVEL_DARKMAGE, TEXT("Layer_Skill"), _float3{ -22.f,8.f,0.f });
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardLaser"), LEVEL_DARKMAGE, TEXT("Layer_Skill"), _float3{ 0.f,11.f,20.f });
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardLaser"), LEVEL_DARKMAGE, TEXT("Layer_Skill"), _float3{ 23.f, 8.f,0.f });

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardGageDown"), LEVEL_DARKMAGE, TEXT("Layer_Skill"), _float3{ 0.f, 2.f,16.f });

		Safe_Release(pGameInstance);

		m_fCreateAcc = 0.f;
	}
}

HRESULT CLevel_BlackMage::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("검은마법사"));

	return S_OK;
}





HRESULT CLevel_BlackMage::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizard"), LEVEL_DARKMAGE, L"Layer_BlackWizard")))
		return E_FAIL;

	

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_BlackMage::Ready_Layer_Npc(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_BlackMage::Ready_Layer_Map(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CSky::SKYDESC SkyDesc;
	SkyDesc.iIndex = 1;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, &SkyDesc)))
		return E_FAIL;

	//CPotal::POTALDESC PotalDesc;
	//PotalDesc.eDestLevel = LEVEL_WHITEMAGE;
	//PotalDesc.Pos = _float3(0.f, 2.f, 0.f);
	//PotalDesc.DestPos = _float3(0.f, 1.f, 0.f);
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Potal"), LEVEL_DARKMAGE, pLayerTag, &PotalDesc)))
	//	return E_FAIL;

	/* 맵 큐브 추가 > 섹션 생성 > 콜리젼 매니저 컴포넌트 멤버함수 호출로 섹션에 큐브 채우기*/

	list<CMap_Manager::CUBEDATA>* pMapData = pGameInstance->ReadMap(L"Map_DarkMage");
	for (auto& Data : *pMapData)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Cube"), LEVEL_DARKMAGE, pLayerTag, &Data)))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);

	if (FAILED(Ready_Layer_Section(TEXT("Layer_Section"))))
		return E_FAIL;

	return S_OK;

}

HRESULT CLevel_BlackMage::Ready_Layer_Section(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CComponent*			pComponent = pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_pColliderCom = (CCollider*)pComponent;


	CMap_Manager::CUBEDATA Data;
	ZeroMemory(&Data, sizeof(CMap_Manager::CUBEDATA));

	for (int j = -5; j < 15; j++)
	{
		for (int i = -5; i < 15; i++)
		{
			for (int y = 0; y < 3; y++)
			{
				_float XIndex = i*5.f;
				_float ZIndex = j*5.f;
				Data.vPos = _float3((-12.f + XIndex) + ZIndex, 2.f + (y * 9.f), -1 - XIndex);
				if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Section"), LEVEL_DARKMAGE, pLayerTag, &Data)))
					return E_FAIL;
			}
		}
	}


	Safe_Release(pGameInstance);


	m_pColliderCom->Set_SectionCubes();

	return S_OK;
}

HRESULT CLevel_BlackMage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardPatternUI"), LEVEL_DARKMAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardHpBar"), LEVEL_DARKMAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardHpGage"), LEVEL_DARKMAGE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);


	return S_OK;
}




CLevel_BlackMage * CLevel_BlackMage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_BlackMage*		pInstance = new CLevel_BlackMage(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : Level_ElenyaEnter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_BlackMage::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}



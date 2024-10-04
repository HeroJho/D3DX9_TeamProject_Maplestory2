#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "GameInstance.h"
#include"Level_Static.h"

#include "Level_GamePlay.h"
#include "Level_Henesys_1.h"
#include "Level_ElenyaEnter.h"
#include "Level_Elenya.h"
#include "Level_ElenyaGAS.h"

#include "Level_DHenesys.h"
#include "Level_DHenesysHunting.h"
#include "Level_DElenyaEnter.h"
#include "Level_DElenya.h"
#include "Level_BlackMageJump.h"
#include "Level_BlackMageEnter.h"
#include "Level_BlackMage.h"
#include "Level_WhiteMage.h"


#include "UIManager.h"
#include "Loader.h"

#include "SpawnerManager.h"


CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	
	CUIManager::Get_Instance()->Start_Loading();

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (true == m_pLoader->Get_Finished())
	{
		
			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CLevel*		pNewLevel = nullptr;

			CSpawnerManager::Get_Instance()->Clear();

			switch (m_eNextLevel)
			{
			case LEVEL_STATIC:
				pNewLevel = CLevel_Static::Create(m_pGraphic_Device);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
				break;
			case LEVEL_HENESYS:
				pNewLevel = CLevel_Henesys_1::Create(m_pGraphic_Device);
				break;
			case LEVEL_ELENYAENTER:
				pNewLevel = CLevel_ElenyaEnter::Create(m_pGraphic_Device);
				break;
			case LEVEL_ELENYA:
				pNewLevel = CLevel_Elenya::Create(m_pGraphic_Device);
				break;
			case LEVEL_GAS:
				pNewLevel = CLevel_ElenyaGAS::Create(m_pGraphic_Device);
				break;

			case LEVEL_DHENESYS:
				pNewLevel = CLevel_DHenesys::Create(m_pGraphic_Device);
				break;
			case LEVEL_DHENESYSHUNTING:
				pNewLevel = CLevel_DHenesysHunting::Create(m_pGraphic_Device);
				break;
			case LEVEL_DELENYAENTER:
				pNewLevel = CLevel_DElenyaEnter::Create(m_pGraphic_Device);
				break;
			case LEVEL_DELENYA:
				pNewLevel = CLevel_DElenya::Create(m_pGraphic_Device);
				break;
			case LEVEL_DARKMAGEJUMP:
				pNewLevel = CLevel_BlackMageJump::Create(m_pGraphic_Device);
				break;
			case LEVEL_DARKMAGEENTER:
				pNewLevel = CLevel_BlackMageEnter::Create(m_pGraphic_Device);
				break;
			case LEVEL_DARKMAGE:
				pNewLevel = CLevel_BlackMage::Create(m_pGraphic_Device);
				break;
			case LEVEL_WHITEMAGE:
				pNewLevel = CLevel_WhiteMage::Create(m_pGraphic_Device);
				break;
			}

			if (nullptr == pNewLevel)
				goto except;

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
				goto except;


			except:
			Safe_Release(pGameInstance);
		/*if (GetKeyState(VK_RETURN) & 0x8000)
		{
		}*/
	}
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed To Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;

}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}



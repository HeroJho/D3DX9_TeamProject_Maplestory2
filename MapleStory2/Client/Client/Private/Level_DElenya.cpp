#include "stdafx.h"
#include "..\Public\Level_DElenya.h"

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

CLevel_DElenya::CLevel_DElenya(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_DElenya::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	


	//if (FAILED(Ready_SkillInfo()))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;


	/*
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;*/

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Npc(TEXT("Layer_Npc"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Spawner(TEXT("Layer_Spawner"))))
		return E_FAIL;*/
	
	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(L"DElanya.mp3", 0.7f);
	
	CUIManager::Get_Instance()->End_Loading();
	CUIManager::Get_Instance()->PlayTitleCurMap(LEVEL_DELENYA);
	// CCutSceneManager::Get_Instance()->Start_Enter_Henesys_2();

	return S_OK;
}

void CLevel_DElenya::Tick(_float fTimeDelta)
{
	if (CQuestManager::Get_Instance()->Set_OrangeMushroom() >= 10)
		CQuestManager::Get_Instance()->QuestClear();
	__super::Tick(fTimeDelta);


	m_fTimeAcc += fTimeDelta;
	if (5.f < m_fTimeAcc)
	{
		CGameInstance::Get_Instance()->PlaySoundW(L"DesBlock.mp3", 4, 1.f);
		m_fTimeAcc = 0.f;
	}

}

HRESULT CLevel_DElenya::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("완전 어려운 점프맵"));

	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;

	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(45.0f);
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.0f;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"), LEVEL_HENESYS, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag)))
	//	return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StoneGolem"), LEVEL_HENESYS, pLayerTag)))
		return E_FAIL;


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_Npc(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_Map(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CSky::SKYDESC SkyDesc;
	SkyDesc.iIndex = 8;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, &SkyDesc)))
		return E_FAIL;

	CPotal::POTALDESC PotalDesc;
	PotalDesc.eDestLevel = LEVEL_DELENYAENTER;
	PotalDesc.Pos = _float3(0.f, 1.f, 0.f);
	PotalDesc.DestPos = _float3(35.f, 16.f, 6.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Potal"), LEVEL_DELENYA, pLayerTag, &PotalDesc)))
		return E_FAIL;

	PotalDesc.eDestLevel = LEVEL_DARKMAGEENTER;
	PotalDesc.Pos = _float3(37.f, 23.f, 3.f);
	PotalDesc.DestPos = _float3(-1.f, 2.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Potal"), LEVEL_DELENYA, pLayerTag, &PotalDesc)))
		return E_FAIL;



	/* 맵 큐브 추가 > 섹션 생성 > 콜리젼 매니저 컴포넌트 멤버함수 호출로 섹션에 큐브 채우기*/

	list<CMap_Manager::CUBEDATA>* pMapData = pGameInstance->ReadMap(L"Map_DElenya");
	for (auto& Data : *pMapData)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_AutoDownCube"), LEVEL_DELENYA, pLayerTag, &Data)))
			return E_FAIL;
	}


	//list<CMap_Manager::MODELDESC>* pModelData = pGameInstance->ReadModel(L"Model_DElenya");
	//if (nullptr != pModelData)
	//{
	//	for (auto& Model : *pModelData)
	//	{
	//		CVIBuffer_Voxel::VOXELDESC VoxDesc;
	//		CopyMemory(VoxDesc.cFileName, Model.cModelName, sizeof(_tchar) * 256);
	//		VoxDesc.vPos = Model.vPos;

	//		_float4x4 Matrix;
	//		D3DXMatrixIdentity(&Matrix);

	//		D3DXMatrixRotationAxis(&Matrix, &_float3(0.f, 1.f, 0.f), D3DXToRadian(45.f));

	//		D3DXVec3TransformCoord((&VoxDesc.vPos), (&VoxDesc.vPos), &Matrix);


	//		VoxDesc.vScale = _float3{ Model.fScale, Model.fScale, Model.fScale };
	//		VoxDesc.vRotationAix = Model.vAix;

	//		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Wood"), LEVEL_DELENYA, pLayerTag, &VoxDesc)))
	//			return E_FAIL;
	//	}

	//}




	Safe_Release(pGameInstance);

	if (FAILED(Ready_Layer_Section(TEXT("Layer_Section"))))
		return E_FAIL;

	return S_OK;

}

HRESULT CLevel_DElenya::Ready_Layer_Section(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CComponent*			pComponent = pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_pColliderCom = (CCollider*)pComponent;


	CMap_Manager::CUBEDATA Data;
	ZeroMemory(&Data, sizeof(CMap_Manager::CUBEDATA));

	for (int y = 0; y < 6; y++)
	{
		for (int j = -5; j < 12; j++)
		{
			for (int i = -5; i < 12; i++)
			{
				_float XIndex = i*5.f;
				_float ZIndex = j*5.f;
				Data.vPos = _float3((-16.f + XIndex) + ZIndex, 2.f + y * 9.f, 7 - XIndex);
				if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Section"), LEVEL_DELENYA, pLayerTag, &Data)))
					return E_FAIL;
			}
		}
	}

	Safe_Release(pGameInstance);


	m_pColliderCom->Set_SectionCubes();

	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);



	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_DElenya::Ready_Layer_Spawner(const _tchar * pLayerTag)
{

	CSpawner::SPAWNERINFO MonsterInfo;

	MonsterInfo.MonsterName = TEXT("TransformSlime");
	MonsterInfo.MonsterPos = _float3{ 5.f , 13.f, 0.f };
	MonsterInfo.SpawnerNum = 0;
	MonsterInfo.MonsterNum = 2;
	MonsterInfo.MonsterColRad = 1.f;
	MonsterInfo.Level = LEVEL_DELENYA;

	CSpawnerManager::Get_Instance()->Add_Spawner(&MonsterInfo);

	MonsterInfo.MonsterName = TEXT("TransformSlime");
	MonsterInfo.MonsterPos = _float3{ 5.f , 13.f, 0.f };
	MonsterInfo.SpawnerNum = 1;
	MonsterInfo.MonsterNum = 2;
	MonsterInfo.MonsterColRad = 1.f;
	MonsterInfo.Level = LEVEL_DELENYA;

	CSpawnerManager::Get_Instance()->Add_Spawner(&MonsterInfo);


	return S_OK;
}

HRESULT CLevel_DElenya::Ready_SkillFrameBtn(const _tchar * pLayerTag)
{
	//스킬Up Btn 간격 가로 144px 세로 40px
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI::UIINFO SkillUpBtnInfo1;
	SkillUpBtnInfo1.fSizeX = 16.f;
	SkillUpBtnInfo1.fSizeY = 14.f;
	SkillUpBtnInfo1.fMoveX = -16.f;
	SkillUpBtnInfo1.fMoveY = -60.f;
	SkillUpBtnInfo1.iNum = 1;

	CUI::UIINFO SkillUpBtnInfo2;
	SkillUpBtnInfo2.fSizeX = 16.f;
	SkillUpBtnInfo2.fSizeY = 14.f;
	SkillUpBtnInfo2.fMoveX = 128.f;
	SkillUpBtnInfo2.fMoveY = -60.f;
	SkillUpBtnInfo2.iNum = 2;

	CUI::UIINFO SkillUpBtnInfo3;
	SkillUpBtnInfo3.fSizeX = 16.f;
	SkillUpBtnInfo3.fSizeY = 14.f;
	SkillUpBtnInfo3.fMoveX = -16.f;
	SkillUpBtnInfo3.fMoveY = -20.f;
	SkillUpBtnInfo3.iNum = 3;

	CUI::UIINFO SkillUpBtnInfo4;
	SkillUpBtnInfo4.fSizeX = 16.f;
	SkillUpBtnInfo4.fSizeY = 14.f;
	SkillUpBtnInfo4.fMoveX = 128.f;
	SkillUpBtnInfo4.fMoveY = -20.f;
	SkillUpBtnInfo4.iNum = 4;

	CUI::UIINFO SkillUpBtnInfo5;
	SkillUpBtnInfo5.fSizeX = 16.f;
	SkillUpBtnInfo5.fSizeY = 14.f;
	SkillUpBtnInfo5.fMoveX = -16.f;
	SkillUpBtnInfo5.fMoveY = 20.f;
	SkillUpBtnInfo5.iNum = 5;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_HENESYS, pLayerTag, &SkillUpBtnInfo1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_HENESYS, pLayerTag, &SkillUpBtnInfo2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_HENESYS, pLayerTag, &SkillUpBtnInfo3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_HENESYS, pLayerTag, &SkillUpBtnInfo4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_HENESYS, pLayerTag, &SkillUpBtnInfo5)))
		return E_FAIL;



	CUI::UIINFO SkillGradeBtnInfo0;
	SkillGradeBtnInfo0.fSizeX = 25.f;
	SkillGradeBtnInfo0.fSizeY = 20.f;
	SkillGradeBtnInfo0.fMoveX = -135.f;
	SkillGradeBtnInfo0.fMoveY = -143.f;

	CUI::UIINFO SkillGradeBtnInfo1;
	SkillGradeBtnInfo1.fSizeX = 25.f;
	SkillGradeBtnInfo1.fSizeY = 20.f;
	SkillGradeBtnInfo1.fMoveX = -108.f;
	SkillGradeBtnInfo1.fMoveY = -143.f;

	CUI::UIINFO SkillGradeBtnInfo2;
	SkillGradeBtnInfo2.fSizeX = 25.f;
	SkillGradeBtnInfo2.fSizeY = 20.f;
	SkillGradeBtnInfo2.fMoveX = -81.f;
	SkillGradeBtnInfo2.fMoveY = -143.f;

	CUI::UIINFO SkillGradeBtnInfo3;
	SkillGradeBtnInfo3.fSizeX = 25.f;
	SkillGradeBtnInfo3.fSizeY = 20.f;
	SkillGradeBtnInfo3.fMoveX = -54.f;
	SkillGradeBtnInfo3.fMoveY = -143.f;

	CUI::UIINFO SkillGradeBtnInfo4;
	SkillGradeBtnInfo4.fSizeX = 25.f;
	SkillGradeBtnInfo4.fSizeY = 20.f;
	SkillGradeBtnInfo4.fMoveX = -27.f;
	SkillGradeBtnInfo4.fMoveY = -143.f;

	CUI::UIINFO SkillGradeBtnInfo5;
	SkillGradeBtnInfo5.fSizeX = 25.f;
	SkillGradeBtnInfo5.fSizeY = 20.f;
	SkillGradeBtnInfo5.fMoveX = 0.f;
	SkillGradeBtnInfo5.fMoveY = -143.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn0"), LEVEL_HENESYS, pLayerTag, &SkillGradeBtnInfo0)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn1"), LEVEL_HENESYS, pLayerTag, &SkillGradeBtnInfo1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn2"), LEVEL_HENESYS, pLayerTag, &SkillGradeBtnInfo2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn3"), LEVEL_HENESYS, pLayerTag, &SkillGradeBtnInfo3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn4"), LEVEL_HENESYS, pLayerTag, &SkillGradeBtnInfo4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn5"), LEVEL_HENESYS, pLayerTag, &SkillGradeBtnInfo5)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_DElenya::Ready_SkillIcon(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI::UIINFO SunCrossInfo;
	SunCrossInfo.fSizeX = 32.f;
	SunCrossInfo.fSizeY = 32.f;
	SunCrossInfo.fMoveX = -129.f;
	SunCrossInfo.fMoveY = -69.f;

	CUI::UIINFO SolunaSlashInfo;
	SolunaSlashInfo.fSizeX = 32.f;
	SolunaSlashInfo.fSizeY = 32.f;
	SolunaSlashInfo.fMoveX = 15.f;
	SolunaSlashInfo.fMoveY = -69.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SunCrossIcon"), LEVEL_HENESYS, pLayerTag, &SunCrossInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SolunaSlashIcon"), LEVEL_HENESYS, pLayerTag, &SolunaSlashInfo)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_DElenya::Ready_SkillInfo()
{
	//CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	//CSunCrossInfo* pSunCross = new CSunCrossInfo;
	//CSolunaSlashInfo* pSolunaSlash = new CSolunaSlashInfo;

	//if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("SunCrossInfo"), CSkillManager::GRADE_BEGENNER, pSunCross)))
	//	return E_FAIL;

	//if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("SolunaSlashInfo"), CSkillManager::GRADE_BEGENNER, pSolunaSlash)))
	//	return E_FAIL;

	return S_OK;


}

HRESULT CLevel_DElenya::Ready_QuickSlot(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//틀어진x 15 x간격 5 y간격 5

	CUI::UIINFO QuickSlot1;
	QuickSlot1.fSizeX = 50.f;
	QuickSlot1.fSizeY = 50.f;
	QuickSlot1.fX = 350.f;
	QuickSlot1.fY = 580.f;

	CUI::UIINFO QuickSlot2;
	QuickSlot2.fSizeX = 50.f;
	QuickSlot2.fSizeY = 50.f;
	QuickSlot2.fX = 365.f;
	QuickSlot2.fY = 635.f;

	CUI::UIINFO QuickSlot3;
	QuickSlot3.fSizeX = 50.f;
	QuickSlot3.fSizeY = 50.f;
	QuickSlot3.fX = 405.f;
	QuickSlot3.fY = 580.f;

	CUI::UIINFO QuickSlot4;
	QuickSlot4.fSizeX = 50.f;
	QuickSlot4.fSizeY = 50.f;
	QuickSlot4.fX = 420.f;
	QuickSlot4.fY = 635.f;

	CUI::UIINFO QuickSlot5;
	QuickSlot5.fSizeX = 50.f;
	QuickSlot5.fSizeY = 50.f;
	QuickSlot5.fX = 460.f;
	QuickSlot5.fY = 580.f;

	CUI::UIINFO QuickSlot6;
	QuickSlot6.fSizeX = 50.f;
	QuickSlot6.fSizeY = 50.f;
	QuickSlot6.fX = 475.f;
	QuickSlot6.fY = 635.f;

	CUI::UIINFO QuickSlot7;
	QuickSlot7.fSizeX = 50.f;
	QuickSlot7.fSizeY = 50.f;
	QuickSlot7.fX = 515.f;
	QuickSlot7.fY = 580.f;

	CUI::UIINFO QuickSlot8;
	QuickSlot8.fSizeX = 50.f;
	QuickSlot8.fSizeY = 50.f;
	QuickSlot8.fX = 530.f;
	QuickSlot8.fY = 635.f;

	CUI::UIINFO QuickSlot9;
	QuickSlot9.fSizeX = 50.f;
	QuickSlot9.fSizeY = 50.f;
	QuickSlot9.fX = 765.f;
	QuickSlot9.fY = 580.f;

	CUI::UIINFO QuickSlot10;
	QuickSlot10.fSizeX = 50.f;
	QuickSlot10.fSizeY = 50.f;
	QuickSlot10.fX = 750.f;
	QuickSlot10.fY = 635.f;

	CUI::UIINFO QuickSlot11;
	QuickSlot11.fSizeX = 50.f;
	QuickSlot11.fSizeY = 50.f;
	QuickSlot11.fX = 820.f;
	QuickSlot11.fY = 580.f;

	CUI::UIINFO QuickSlot12;
	QuickSlot12.fSizeX = 50.f;
	QuickSlot12.fSizeY = 50.f;
	QuickSlot12.fX = 805.f;
	QuickSlot12.fY = 635.f;

	CUI::UIINFO QuickSlot13;
	QuickSlot13.fSizeX = 50.f;
	QuickSlot13.fSizeY = 50.f;
	QuickSlot13.fX = 875.f;
	QuickSlot13.fY = 580.f;

	CUI::UIINFO QuickSlot14;
	QuickSlot14.fSizeX = 50.f;
	QuickSlot14.fSizeY = 50.f;
	QuickSlot14.fX = 860.f;
	QuickSlot14.fY = 635.f;

	CUI::UIINFO QuickSlot15;
	QuickSlot15.fSizeX = 50.f;
	QuickSlot15.fSizeY = 50.f;
	QuickSlot15.fX = 930.f;
	QuickSlot15.fY = 580.f;

	CUI::UIINFO QuickSlot16;
	QuickSlot16.fSizeX = 50.f;
	QuickSlot16.fSizeY = 50.f;
	QuickSlot16.fX = 915.f;
	QuickSlot16.fY = 635.f;



	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot6)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot7)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot8)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot9)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot10)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot11)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot12)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot13)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot14)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot15)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_HENESYS, pLayerTag, &QuickSlot16)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_DElenya::Ready_QuickSlotSkill(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI::UIINFO QuickSlotSkill1;
	QuickSlotSkill1.fSizeX = 40.f;
	QuickSlotSkill1.fSizeY = 40.f;
	QuickSlotSkill1.fX = 350.f;
	QuickSlotSkill1.fY = 580.f;
	QuickSlotSkill1.iNum = 0;

	CUI::UIINFO QuickSlotSkill2;
	QuickSlotSkill2.fSizeX = 40.f;
	QuickSlotSkill2.fSizeY = 40.f;
	QuickSlotSkill2.fX = 365.f;
	QuickSlotSkill2.fY = 635.f;
	QuickSlotSkill1.iNum = 1;

	CUI::UIINFO QuickSlotSkill3;
	QuickSlotSkill3.fSizeX = 40.f;
	QuickSlotSkill3.fSizeY = 40.f;
	QuickSlotSkill3.fX = 405.f;
	QuickSlotSkill3.fY = 580.f;
	QuickSlotSkill1.iNum = 2;

	CUI::UIINFO QuickSlotSkill4;
	QuickSlotSkill4.fSizeX = 40.f;
	QuickSlotSkill4.fSizeY = 40.f;
	QuickSlotSkill4.fX = 420.f;
	QuickSlotSkill4.fY = 635.f;
	QuickSlotSkill1.iNum = 3;

	CUI::UIINFO QuickSlotSkill5;
	QuickSlotSkill5.fSizeX = 40.f;
	QuickSlotSkill5.fSizeY = 40.f;
	QuickSlotSkill5.fX = 460.f;
	QuickSlotSkill5.fY = 580.f;
	QuickSlotSkill1.iNum = 4;

	CUI::UIINFO QuickSlotSkill6;
	QuickSlotSkill6.fSizeX = 40.f;
	QuickSlotSkill6.fSizeY = 40.f;
	QuickSlotSkill6.fX = 475.f;
	QuickSlotSkill6.fY = 635.f;
	QuickSlotSkill1.iNum = 5;

	CUI::UIINFO QuickSlotSkill7;
	QuickSlotSkill7.fSizeX = 40.f;
	QuickSlotSkill7.fSizeY = 40.f;
	QuickSlotSkill7.fX = 515.f;
	QuickSlotSkill7.fY = 580.f;
	QuickSlotSkill1.iNum = 6;

	CUI::UIINFO QuickSlotSkill8;
	QuickSlotSkill8.fSizeX = 40.f;
	QuickSlotSkill8.fSizeY = 40.f;
	QuickSlotSkill8.fX = 530.f;
	QuickSlotSkill8.fY = 635.f;
	QuickSlotSkill1.iNum = 7;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill6)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill7)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_HENESYS, pLayerTag, &QuickSlotSkill8)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_DElenya * CLevel_DElenya::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_DElenya*		pInstance = new CLevel_DElenya(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : Level_ElenyaEnter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_DElenya::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}



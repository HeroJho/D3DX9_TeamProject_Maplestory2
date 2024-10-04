#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "ItemInfo.h"
#include "DefaultInfo.h"
#include "RedPortionInfo.h"
#include "BluePortionInfo.h"
#include "SkillInfo.h"
#include "SunCrossInfo.h"
#include "SolunaSlashInfo.h"
#include "WarriorReefInfo.h"
#include "ReefAttackInfo.h"
#include "SpearPullingInfo.h"
#include "CardinalBlastInfo.h"
#include "ChasingShotInfo.h"
#include "BeastInfo.h"
#include "SkillManager.h"
#include "InvenManager.h"
#include "UI.h"
#include "QuestManager.h"
#include "SpawnerManager.h"
#include "Maya.h"
#include "Level_Loading.h"
#include "MainApp.h"
#include "Potal.h"
#include "ConsumIcon.h"
#include "GreenMushroomInfo.h"
#include "BlueSnailInfo.h"
#include "RedSnailInfo.h"
#include "SlimeInfo.h"
#include "GoStumpInfo.h"
#include "RibbonPigInfo.h"
#include "CutSceneManager.h"
#include "ItemNotice.h"
#include "UIManager.h"
#include "BlackWizardPatternUI.h"
#include "TeleportInfo.h"
#include "Sky.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	//CGameInstance::Get_Instance()->PlaySound(L"Test.wav",1, 1.f);

	if (g_bStaticClone == false)
	{
		if (FAILED(Ready_SkillInfo()))
			return E_FAIL;

		if (FAILED(Ready_EquipItemInfo()))
			return E_FAIL;

		if (FAILED(Ready_ConsumItemInfo()))
			return E_FAIL;

		if (FAILED(Ready_StuffItemInfo()))
			return E_FAIL;

		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
			return E_FAIL;
		
	}


	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Npc(TEXT("Layer_Npc"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Spawner(TEXT("Layer_Spawner"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_NonStatic_UI(TEXT("Layer_NonStatic_UI"))))
		return E_FAIL;

	g_bStaticClone = true;

	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(L"FloralLife.mp3",1.f);

	CUIManager::Get_Instance()->End_Loading();
	CUIManager::Get_Instance()->PlayTitleCurMap(LEVEL_GAMEPLAY);
	CCutSceneManager::Get_Instance()->Start_Enter_Henesys_1();
	// CCutSceneManager::Get_Instance()->Start_Enter_GASHenesys();


	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	if (CQuestManager::Get_Instance()->Set_OrangeMushroom() >= 10)
		CQuestManager::Get_Instance()->QuestClear();
	else if (CQuestManager::Get_Instance()->Set_StoneGolem() >= 1)
		CQuestManager::Get_Instance()->QuestClear();
	//__super::Tick(fTimeDelta);
	
	__super::Tick(fTimeDelta);

	g_bGamePlay = true;


}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Camera_Free"), LEVEL_STATIC, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Player"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Npc(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (CCutSceneManager::Get_Instance()->Get_jangRander())
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ElderStan"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Maya"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Helena"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Ming"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Bird"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GgoGgo"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	CCreature::CRETUREDESC Creatureinfo;
	Creatureinfo.vPos = _float3(11.5f, 5.7f, -3.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Chick"), LEVEL_GAMEPLAY, pLayerTag,&Creatureinfo)))
		return E_FAIL;
	CCreature::CRETUREDESC Creatureinfo1;
	Creatureinfo1.vPos = _float3(11.8f, 5.7f, -3.7f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Chick"), LEVEL_GAMEPLAY, pLayerTag, &Creatureinfo1)))
		return E_FAIL;
	CCreature::CRETUREDESC Creatureinfo2;
	Creatureinfo2.vPos = _float3(12.1f, 5.7f, -3.9f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Chick"), LEVEL_GAMEPLAY, pLayerTag, &Creatureinfo2)))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Taxi"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Map(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);





	CCreature::CRETUREDESC WeedInfo0;
	WeedInfo0.vPos = _float3(-9.f, 0.68f, -0.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo0)))
		return E_FAIL;
	CCreature::CRETUREDESC WeedInfo1;
	WeedInfo1.vPos = _float3(-8.5f, 0.68f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo1)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo2;
	WeedInfo2.vPos = _float3(-3.8f, 0.68f, 0.7f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo2)))
		return E_FAIL;
	CCreature::CRETUREDESC WeedInfo3;
	WeedInfo3.vPos = _float3(-3.3f, 0.68f, 0.5f);
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo3)))
		//return E_FAIL;

	CCreature::CRETUREDESC WeedInfo4;
	WeedInfo4.vPos = _float3(-5.7f, 0.68f, 2.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo4)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo7;
	WeedInfo7.vPos = _float3(-0.3f, 3.68f, 3.0f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo7)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo8;
	WeedInfo8.vPos = _float3(-3.8f, 3.68f, 6.8f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo8)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo9;
	WeedInfo9.vPos = _float3(1.3f, 3.68f, 9.0f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo9)))
		return E_FAIL;
	CCreature::CRETUREDESC WeedInfo10;
	WeedInfo10.vPos = _float3(1.5f, 3.68f, 7.4f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo10)))
		return E_FAIL;

	CCreature::CRETUREDESC GrassInfo1;
	GrassInfo1.vPos = _float3(4.8f, 3.78f, 4.8f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassInfo1)))
		return E_FAIL;

	CCreature::CRETUREDESC GrassInfo2;
	GrassInfo2.vPos = _float3(0.f, 2.78f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassInfo2)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo11;
	WeedInfo11.vPos = _float3(0.f, 0.68f, -8.0f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo11)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo12;
	WeedInfo12.vPos = _float3(10.f, 5.68f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo12)))
		return E_FAIL;
	CCreature::CRETUREDESC WeedInfo13;
	WeedInfo13.vPos = _float3(13.f, 5.68f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo13)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo14;
	WeedInfo14.vPos = _float3(17.f, 5.68f, -7.4f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo14)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo15;
	WeedInfo15.vPos = _float3(26.f, 3.68f, -22.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo15)))
		return E_FAIL;
	CCreature::CRETUREDESC WeedInfo16;
	WeedInfo16.vPos = _float3(26.f, 3.68f, -7.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo16)))
		return E_FAIL;
	CCreature::CRETUREDESC WeedInfo17;
	WeedInfo17.vPos = _float3(26.f, 3.68f, -11.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo17)))
		return E_FAIL;
	CCreature::CRETUREDESC WeedInfo18;
	WeedInfo18.vPos = _float3(29.f, 3.68f, -16.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo18)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo19;
	WeedInfo19.vPos = _float3(26.f, 3.68f, -17.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo19)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo20;
	WeedInfo20.vPos = _float3(24.f, 3.68f, -15.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo20)))
		return E_FAIL;




	CCreature::CRETUREDESC GrassInfo3;
	GrassInfo3.vPos = _float3(20.f, 3.78f, -13.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassInfo3)))
		return E_FAIL;

	CCreature::CRETUREDESC GrassInfo4;
	GrassInfo4.vPos = _float3(23.f, 3.78f, -19.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassInfo4)))
		return E_FAIL;


	CCreature::CRETUREDESC GrassInfo6;
	GrassInfo6.vPos = _float3(32.f, 3.78f, -13.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassInfo6)))
		return E_FAIL;

	CCreature::CRETUREDESC WeedInfo21;
	WeedInfo21.vPos = _float3(-5.f, 0.68f, -9.0f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo21)))
		return E_FAIL;	CCreature::CRETUREDESC WeedInfo22;
	WeedInfo22.vPos = _float3(1.f, 0.68f, -10.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Weed"), LEVEL_GAMEPLAY, pLayerTag, &WeedInfo22)))
		return E_FAIL;

	CCreature::CRETUREDESC GrassInfo7;
	GrassInfo7.vPos = _float3(-8.f, 0.78f, -6.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassInfo7)))
		return E_FAIL;


	CCreature::CRETUREDESC GrassInfo8;
	GrassInfo8.vPos = _float3(-2.5f, 0.78f, -10.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassInfo8)))
		return E_FAIL;

	CPotal::POTALDESC PotalDesc;
	PotalDesc.eDestLevel = LEVEL_HENESYS;
	PotalDesc.Pos = _float3(29.f, 4.f, -12.f);
	PotalDesc.DestPos = _float3(-2.f, 2.5f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Potal"), LEVEL_GAMEPLAY, pLayerTag, &PotalDesc)))
		return E_FAIL;


	//PotalDesc.eDestLevel = LEVEL_DARKMAGE;
	//PotalDesc.Pos = _float3(-1.f, 1.f, -3.f);
	////PotalDesc.DestPos = _float3(-16.f, 1.f, -16.f);
	//PotalDesc.DestPos = _float3(-2.f, 1.f, -2.f);
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Potal"), LEVEL_GAMEPLAY, pLayerTag, &PotalDesc)))
	//	return E_FAIL;

	CSky::SKYDESC SkyDesc;
	SkyDesc.iIndex = 5;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, &SkyDesc)))
		return E_FAIL;
	



	



	list<CMap_Manager::CUBEDATA>* pMapData = pGameInstance->ReadMap(L"Map_Henesys");
	for (auto& Data : *pMapData)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Cube"), LEVEL_GAMEPLAY, pLayerTag, &Data)))
			return E_FAIL;
	}


	list<CMap_Manager::MODELDESC>* pModelData = pGameInstance->ReadModel(L"Total_Henesys");
	if (nullptr != pModelData)
	{
		for (auto& Model : *pModelData)
		{
			CVIBuffer_Voxel::VOXELDESC VoxDesc;
			CopyMemory(VoxDesc.cFileName, Model.cModelName, sizeof(_tchar) * 256);
			VoxDesc.vPos = Model.vPos;

			_float4x4 Matrix;
			D3DXMatrixIdentity(&Matrix);

			D3DXMatrixRotationAxis(&Matrix, &_float3(0.f, 1.f, 0.f), D3DXToRadian(45.f));

			D3DXVec3TransformCoord((&VoxDesc.vPos), (&VoxDesc.vPos), &Matrix);


			VoxDesc.vScale = _float3{ Model.fScale, Model.fScale, Model.fScale };
			VoxDesc.vRotationAix = Model.vAix;

			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Wood"), LEVEL_GAMEPLAY, pLayerTag, &VoxDesc)))
				return E_FAIL;
		}

	}

	if (FAILED(Ready_Layer_Section(TEXT("Layer_Section"))))
		return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Section(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CComponent*			pComponent = pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_pColliderCom = (CCollider*)pComponent;



	CMap_Manager::CUBEDATA Data;
	ZeroMemory(&Data, sizeof(CMap_Manager::CUBEDATA));


	for (int y = 0; y < 3; y++)
	{

		for (int j = -5; j < 10; j++)
		{
			for (int i = -5; i < 10; i++)
			{
				_float XIndex = i*5.f;
				_float ZIndex = j*5.f;
				Data.vPos = _float3((-12.f + XIndex) + ZIndex, 1 + y * 9.f, -1 - XIndex);
				if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Section"), LEVEL_GAMEPLAY, pLayerTag, &Data)))
					return E_FAIL;
			}
		}

	}

	
	Safe_Release(pGameInstance);


	m_pColliderCom->Set_SectionCubes();
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CUI::UIINFO SkillFrameiInfo;
	SkillFrameiInfo.fSizeX = 318.f;
	SkillFrameiInfo.fSizeY = 360.f;
	SkillFrameiInfo.fX = 900.f;
	SkillFrameiInfo.fY = 300.f;

	CUI::UIINFO InvenFrameiInfo;
	InvenFrameiInfo.fSizeX = 197.f;
	InvenFrameiInfo.fSizeY = 380.f;
	InvenFrameiInfo.fX = 750.f;
	InvenFrameiInfo.fY = 250.f;

	

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuestUI"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_InvenFrame"), LEVEL_STATIC, pLayerTag, &InvenFrameiInfo)))
		return E_FAIL;

	Ready_InvenBtn(pLayerTag);

	Ready_ConsumItemIcon(pLayerTag);

	Ready_StuffItemIcon(pLayerTag);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillFrame"), LEVEL_STATIC, pLayerTag, &SkillFrameiInfo)))
		return E_FAIL;

	Ready_SkillFrameBtn(pLayerTag);

	Ready_SkillIcon(pLayerTag);



	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_HpBarBase"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_HpBarHp"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_HpBarMp"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_HpBarIcon"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ExpBase"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ExpBar"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ExpLine"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_MenuIcon"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;



	Ready_QuickSlot(pLayerTag);

	Ready_QuickSlotSkill(pLayerTag);

	Ready_QuickSlotItem(pLayerTag);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_MouseSkillIcon"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_MouseItemIcon"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ItemNotice"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Chat"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ChatBox"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;


	CGameObject* pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TaxiChatBox"), LEVEL_STATIC, pLayerTag, &pObj, nullptr)))
		return E_FAIL;
	CUIManager::Get_Instance()->SetTaxiChatBox(pObj);




	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_MouseCursor"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;





	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_NonStatic_UI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Bulb"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Spawner(const _tchar * pLayerTag)
{


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_InvenBtn(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI::UIINFO EquipInfo;
	EquipInfo.fSizeX = 29.f;
	EquipInfo.fSizeY = 17.f;
	EquipInfo.fMoveX = -70.f;
	EquipInfo.fMoveY = -153.f;
	EquipInfo.iNum = 1;

	CUI::UIINFO ConsumInfo;
	ConsumInfo.fSizeX = 29.f;
	ConsumInfo.fSizeY = 17.f;
	ConsumInfo.fMoveX = -39.f;
	ConsumInfo.fMoveY = -153.f;
	ConsumInfo.iNum = 2;

	CUI::UIINFO StuffInfo;
	StuffInfo.fSizeX = 29.f;
	StuffInfo.fSizeY = 17.f;
	StuffInfo.fMoveX = -8.f;
	StuffInfo.fMoveY = -153.f;
	StuffInfo.iNum = 3;

	CUI::UIINFO InstallInfo;
	InstallInfo.fSizeX = 29.f;
	InstallInfo.fSizeY = 17.f;
	InstallInfo.fMoveX = 23.f;
	InstallInfo.fMoveY = -153.f;
	InstallInfo.iNum = 4;

	CUI::UIINFO CashInfo;
	CashInfo.fSizeX = 29.f;
	CashInfo.fSizeY = 17.f;
	CashInfo.fMoveX = 54.f;
	CashInfo.fMoveY = -153.f;
	CashInfo.iNum = 5;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_EquipBtn"), LEVEL_STATIC, pLayerTag, &EquipInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumBtn"), LEVEL_STATIC, pLayerTag, &ConsumInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffBtn"), LEVEL_STATIC, pLayerTag, &StuffInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_InstallBtn"), LEVEL_STATIC, pLayerTag, &InstallInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_CashBtn"), LEVEL_STATIC, pLayerTag, &CashInfo)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_EquipItemInfo()
{
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	CDefaultInfo* pDefaultEquip = new CDefaultInfo;
	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("DefaultInfo"), CInvenManager::TYPE_EQUIP, pDefaultEquip)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_ConsumItemInfo()
{
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();

	CDefaultInfo* pDefaultConsum = new CDefaultInfo;
	CRedPortionInfo* pRedPortion = new CRedPortionInfo;
	CBluePortionInfo* pBluePortion = new CBluePortionInfo;
	CTeleportInfo* pTeleport = new CTeleportInfo;

	
	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("DefaultInfo"), CInvenManager::TYPE_CONSUM, pDefaultConsum)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("RedPortionInfo"), CInvenManager::TYPE_CONSUM, pRedPortion)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("BluePortionInfo"), CInvenManager::TYPE_CONSUM, pBluePortion)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("TeleportInfo"), CInvenManager::TYPE_CONSUM, pTeleport)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_StuffItemInfo()
{
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	
	CDefaultInfo* pDefaultStuff = new CDefaultInfo;
	CGreenMushroomInfo* pGreenMushroomInfo = new CGreenMushroomInfo;
	CBlueSnailInfo* pBlueSnailInfo = new CBlueSnailInfo;
	CRedSnailInfo* pRedSnailInfo = new CRedSnailInfo;
	CSlimeInfo* pSlimeInfo = new CSlimeInfo;
	CRibbonPigInfo* pRibbonPigInfo = new CRibbonPigInfo;
	CGoStumpInfo* pGostumpInfo = new CGoStumpInfo;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("DefaultInfo"), CInvenManager::TYPE_STUFF, pDefaultStuff)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("GreenMushroomInfo"), CInvenManager::TYPE_STUFF, pGreenMushroomInfo)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("BlueSnailInfo"), CInvenManager::TYPE_STUFF, pBlueSnailInfo)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("RedSnailInfo"), CInvenManager::TYPE_STUFF, pRedSnailInfo)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("SlimeInfo"), CInvenManager::TYPE_STUFF, pSlimeInfo)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("RibbonPigInfo"), CInvenManager::TYPE_STUFF, pRibbonPigInfo)))
		return E_FAIL;

	if (FAILED(pInvenInstance->Add_ItemInfo(TEXT("GoStumpInfo"), CInvenManager::TYPE_STUFF, pGostumpInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_EquipItemIcon(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	//x�� 42 y�� 42
	CUI::UIINFO ConsumIcon1;
	ConsumIcon1.fSizeX = 27.f;
	ConsumIcon1.fSizeY = 27.f;
	ConsumIcon1.fMoveX = -69.f;
	ConsumIcon1.fMoveY = -118.f;
	ConsumIcon1.iNum = 1;
	ConsumIcon1.iTextNum = 0;
	ConsumIcon1.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon2;
	ConsumIcon2.fSizeX = 27.f;
	ConsumIcon2.fSizeY = 27.f;
	ConsumIcon2.fMoveX = -27.f;
	ConsumIcon2.fMoveY = -118.f;
	ConsumIcon2.iNum = 2;
	ConsumIcon2.iTextNum = 0;
	ConsumIcon2.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon3;
	ConsumIcon3.fSizeX = 27.f;
	ConsumIcon3.fSizeY = 27.f;
	ConsumIcon3.fMoveX = 15.f;
	ConsumIcon3.fMoveY = -118.f;
	ConsumIcon3.iNum = 3;
	ConsumIcon3.iTextNum = 0;
	ConsumIcon3.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon4;
	ConsumIcon4.fSizeX = 27.f;
	ConsumIcon4.fSizeY = 27.f;
	ConsumIcon4.fMoveX = 57.f;
	ConsumIcon4.fMoveY = -118.f;
	ConsumIcon4.iNum = 4;
	ConsumIcon4.iTextNum = 0;
	ConsumIcon4.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon5;
	ConsumIcon5.fSizeX = 27.f;
	ConsumIcon5.fSizeY = 27.f;
	ConsumIcon5.fMoveX = -69.f;
	ConsumIcon5.fMoveY = -76.f;
	ConsumIcon5.iNum = 5;
	ConsumIcon5.iTextNum = 0;
	ConsumIcon5.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon6;
	ConsumIcon6.fSizeX = 27.f;
	ConsumIcon6.fSizeY = 27.f;
	ConsumIcon6.fMoveX = -27.f;
	ConsumIcon6.fMoveY = -76.f;
	ConsumIcon6.iNum = 6;
	ConsumIcon6.iTextNum = 0;
	ConsumIcon6.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon7;
	ConsumIcon7.fSizeX = 27.f;
	ConsumIcon7.fSizeY = 27.f;
	ConsumIcon7.fMoveX = 15.f;
	ConsumIcon7.fMoveY = -76.f;
	ConsumIcon7.iNum = 7;
	ConsumIcon7.iTextNum = 0;
	ConsumIcon7.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon8;
	ConsumIcon8.fSizeX = 27.f;
	ConsumIcon8.fSizeY = 27.f;
	ConsumIcon8.fMoveX = 57.f;
	ConsumIcon8.fMoveY = -76.f;
	ConsumIcon8.iNum = 8;
	ConsumIcon8.iTextNum = 0;
	ConsumIcon8.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon9;
	ConsumIcon9.fSizeX = 27.f;
	ConsumIcon9.fSizeY = 27.f;
	ConsumIcon9.fMoveX = -69.f;
	ConsumIcon9.fMoveY = -34.f;
	ConsumIcon9.iNum = 9;
	ConsumIcon9.iTextNum = 0;
	ConsumIcon9.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon10;
	ConsumIcon10.fSizeX = 27.f;
	ConsumIcon10.fSizeY = 27.f;
	ConsumIcon10.fMoveX = -27.f;
	ConsumIcon10.fMoveY = -34.f;
	ConsumIcon10.iNum = 10;
	ConsumIcon10.iTextNum = 0;
	ConsumIcon10.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon11;
	ConsumIcon11.fSizeX = 27.f;
	ConsumIcon11.fSizeY = 27.f;
	ConsumIcon11.fMoveX = 15.f;
	ConsumIcon11.fMoveY = -34.f;
	ConsumIcon11.iNum = 11;
	ConsumIcon11.iTextNum = 0;
	ConsumIcon11.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon12;
	ConsumIcon12.fSizeX = 27.f;
	ConsumIcon12.fSizeY = 27.f;
	ConsumIcon12.fMoveX = 57.f;
	ConsumIcon12.fMoveY = -34.f;
	ConsumIcon12.iNum = 12;
	ConsumIcon12.iTextNum = 0;
	ConsumIcon12.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon13;
	ConsumIcon13.fSizeX = 27.f;
	ConsumIcon13.fSizeY = 27.f;
	ConsumIcon13.fMoveX = -69.f;
	ConsumIcon13.fMoveY = 8.f;
	ConsumIcon13.iNum = 13;
	ConsumIcon13.iTextNum = 0;
	ConsumIcon13.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon14;
	ConsumIcon14.fSizeX = 27.f;
	ConsumIcon14.fSizeY = 27.f;
	ConsumIcon14.fMoveX = -27.f;
	ConsumIcon14.fMoveY = 8.f;
	ConsumIcon14.iNum = 14;
	ConsumIcon14.iTextNum = 0;
	ConsumIcon14.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon15;
	ConsumIcon15.fSizeX = 27.f;
	ConsumIcon15.fSizeY = 27.f;
	ConsumIcon15.fMoveX = 15.f;
	ConsumIcon15.fMoveY = 8.f;
	ConsumIcon15.iNum = 15;
	ConsumIcon15.iTextNum = 0;
	ConsumIcon15.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon16;
	ConsumIcon16.fSizeX = 27.f;
	ConsumIcon16.fSizeY = 27.f;
	ConsumIcon16.fMoveX = 57.f;
	ConsumIcon16.fMoveY = 8.f;
	ConsumIcon16.iNum = 16;
	ConsumIcon16.iTextNum = 0;
	ConsumIcon16.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon17;
	ConsumIcon17.fSizeX = 27.f;
	ConsumIcon17.fSizeY = 27.f;
	ConsumIcon17.fMoveX = -69.f;
	ConsumIcon17.fMoveY = 50.f;
	ConsumIcon17.iNum = 17;
	ConsumIcon17.iTextNum = 0;
	ConsumIcon17.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon18;
	ConsumIcon18.fSizeX = 27.f;
	ConsumIcon18.fSizeY = 27.f;
	ConsumIcon18.fMoveX = -27.f;
	ConsumIcon18.fMoveY = 50.f;
	ConsumIcon18.iNum = 18;
	ConsumIcon18.iTextNum = 0;
	ConsumIcon18.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon19;
	ConsumIcon19.fSizeX = 27.f;
	ConsumIcon19.fSizeY = 27.f;
	ConsumIcon19.fMoveX = 15.f;
	ConsumIcon19.fMoveY = 50.f;
	ConsumIcon19.iNum = 19;
	ConsumIcon19.iTextNum = 0;
	ConsumIcon19.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon20;
	ConsumIcon20.fSizeX = 27.f;
	ConsumIcon20.fSizeY = 27.f;
	ConsumIcon20.fMoveX = 57.f;
	ConsumIcon20.fMoveY = 50.f;
	ConsumIcon20.iNum = 20;
	ConsumIcon20.iTextNum = 0;
	ConsumIcon20.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon21;
	ConsumIcon21.fSizeX = 27.f;
	ConsumIcon21.fSizeY = 27.f;
	ConsumIcon21.fMoveX = -69.f;
	ConsumIcon21.fMoveY = 92.f;
	ConsumIcon21.iNum = 21;
	ConsumIcon21.iTextNum = 0;
	ConsumIcon21.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon22;
	ConsumIcon22.fSizeX = 27.f;
	ConsumIcon22.fSizeY = 27.f;
	ConsumIcon22.fMoveX = -27.f;
	ConsumIcon22.fMoveY = 92.f;
	ConsumIcon22.iNum = 22;
	ConsumIcon22.iTextNum = 0;
	ConsumIcon22.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon23;
	ConsumIcon23.fSizeX = 27.f;
	ConsumIcon23.fSizeY = 27.f;
	ConsumIcon23.fMoveX = 15.f;
	ConsumIcon23.fMoveY = 92.f;
	ConsumIcon23.iNum = 23;
	ConsumIcon23.iTextNum = 0;
	ConsumIcon23.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon24;
	ConsumIcon24.fSizeX = 27.f;
	ConsumIcon24.fSizeY = 27.f;
	ConsumIcon24.fMoveX = 57.f;
	ConsumIcon24.fMoveY = 92.f;
	ConsumIcon24.iNum = 24;
	ConsumIcon24.iTextNum = 0;
	ConsumIcon24.pTag = L"DefaultInfo";

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon6)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon7)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon8)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon9)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon10)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon11)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon12)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon13)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon14)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon15)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon16)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon17)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon18)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon19)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon20)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon21)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon22)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon23)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon24)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_ConsumItemIcon(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	//x�� 42 y�� 42
	CUI::UIINFO ConsumIcon1;
	ConsumIcon1.fSizeX = 27.f;
	ConsumIcon1.fSizeY = 27.f;
	ConsumIcon1.fMoveX = -69.f;
	ConsumIcon1.fMoveY = -118.f;
	ConsumIcon1.iNum = 1;
	ConsumIcon1.iTextNum = 0;
	ConsumIcon1.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon2;
	ConsumIcon2.fSizeX = 27.f;
	ConsumIcon2.fSizeY = 27.f;
	ConsumIcon2.fMoveX = -27.f;
	ConsumIcon2.fMoveY = -118.f;
	ConsumIcon2.iNum = 2;
	ConsumIcon2.iTextNum = 0;
	ConsumIcon2.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon3;
	ConsumIcon3.fSizeX = 27.f;
	ConsumIcon3.fSizeY = 27.f;
	ConsumIcon3.fMoveX = 15.f;
	ConsumIcon3.fMoveY = -118.f;
	ConsumIcon3.iNum = 3;
	ConsumIcon3.iTextNum = 0;
	ConsumIcon3.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon4;
	ConsumIcon4.fSizeX = 27.f;
	ConsumIcon4.fSizeY = 27.f;
	ConsumIcon4.fMoveX = 57.f;
	ConsumIcon4.fMoveY = -118.f;
	ConsumIcon4.iNum = 4;
	ConsumIcon4.iTextNum = 0;
	ConsumIcon4.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon5;
	ConsumIcon5.fSizeX = 27.f;
	ConsumIcon5.fSizeY = 27.f;
	ConsumIcon5.fMoveX = -69.f;
	ConsumIcon5.fMoveY = -76.f;
	ConsumIcon5.iNum = 5;
	ConsumIcon5.iTextNum = 0;
	ConsumIcon5.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon6;
	ConsumIcon6.fSizeX = 27.f;
	ConsumIcon6.fSizeY = 27.f;
	ConsumIcon6.fMoveX = -27.f;
	ConsumIcon6.fMoveY = -76.f;
	ConsumIcon6.iNum = 6;
	ConsumIcon6.iTextNum = 0;
	ConsumIcon6.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon7;
	ConsumIcon7.fSizeX = 27.f;
	ConsumIcon7.fSizeY = 27.f;
	ConsumIcon7.fMoveX = 15.f;
	ConsumIcon7.fMoveY = -76.f;
	ConsumIcon7.iNum = 7;
	ConsumIcon7.iTextNum = 0;
	ConsumIcon7.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon8;
	ConsumIcon8.fSizeX = 27.f;
	ConsumIcon8.fSizeY = 27.f;
	ConsumIcon8.fMoveX = 57.f;
	ConsumIcon8.fMoveY = -76.f;
	ConsumIcon8.iNum = 8;
	ConsumIcon8.iTextNum = 0;
	ConsumIcon8.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon9;
	ConsumIcon9.fSizeX = 27.f;
	ConsumIcon9.fSizeY = 27.f;
	ConsumIcon9.fMoveX = -69.f;
	ConsumIcon9.fMoveY = -34.f;
	ConsumIcon9.iNum = 9;
	ConsumIcon9.iTextNum = 0;
	ConsumIcon9.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon10;
	ConsumIcon10.fSizeX = 27.f;
	ConsumIcon10.fSizeY = 27.f;
	ConsumIcon10.fMoveX = -27.f;
	ConsumIcon10.fMoveY = -34.f;
	ConsumIcon10.iNum = 10;
	ConsumIcon10.iTextNum = 0;
	ConsumIcon10.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon11;
	ConsumIcon11.fSizeX = 27.f;
	ConsumIcon11.fSizeY = 27.f;
	ConsumIcon11.fMoveX = 15.f;
	ConsumIcon11.fMoveY = -34.f;
	ConsumIcon11.iNum = 11;
	ConsumIcon11.iTextNum = 0;
	ConsumIcon11.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon12;
	ConsumIcon12.fSizeX = 27.f;
	ConsumIcon12.fSizeY = 27.f;
	ConsumIcon12.fMoveX = 57.f;
	ConsumIcon12.fMoveY = -34.f;
	ConsumIcon12.iNum = 12;
	ConsumIcon12.iTextNum = 0;
	ConsumIcon12.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon13;
	ConsumIcon13.fSizeX = 27.f;
	ConsumIcon13.fSizeY = 27.f;
	ConsumIcon13.fMoveX = -69.f;
	ConsumIcon13.fMoveY = 8.f;
	ConsumIcon13.iNum = 13;
	ConsumIcon13.iTextNum = 0;
	ConsumIcon13.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon14;
	ConsumIcon14.fSizeX = 27.f;
	ConsumIcon14.fSizeY = 27.f;
	ConsumIcon14.fMoveX = -27.f;
	ConsumIcon14.fMoveY = 8.f;
	ConsumIcon14.iNum = 14;
	ConsumIcon14.iTextNum = 0;
	ConsumIcon14.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon15;
	ConsumIcon15.fSizeX = 27.f;
	ConsumIcon15.fSizeY = 27.f;
	ConsumIcon15.fMoveX = 15.f;
	ConsumIcon15.fMoveY = 8.f;
	ConsumIcon15.iNum = 15;
	ConsumIcon15.iTextNum = 0;
	ConsumIcon15.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon16;
	ConsumIcon16.fSizeX = 27.f;
	ConsumIcon16.fSizeY = 27.f;
	ConsumIcon16.fMoveX = 57.f;
	ConsumIcon16.fMoveY = 8.f;
	ConsumIcon16.iNum = 16;
	ConsumIcon16.iTextNum = 0;
	ConsumIcon16.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon17;
	ConsumIcon17.fSizeX = 27.f;
	ConsumIcon17.fSizeY = 27.f;
	ConsumIcon17.fMoveX = -69.f;
	ConsumIcon17.fMoveY = 50.f;
	ConsumIcon17.iNum = 17;
	ConsumIcon17.iTextNum = 0;
	ConsumIcon17.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon18;
	ConsumIcon18.fSizeX = 27.f;
	ConsumIcon18.fSizeY = 27.f;
	ConsumIcon18.fMoveX = -27.f;
	ConsumIcon18.fMoveY = 50.f;
	ConsumIcon18.iNum = 18;
	ConsumIcon18.iTextNum = 0;
	ConsumIcon18.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon19;
	ConsumIcon19.fSizeX = 27.f;
	ConsumIcon19.fSizeY = 27.f;
	ConsumIcon19.fMoveX = 15.f;
	ConsumIcon19.fMoveY = 50.f;
	ConsumIcon19.iNum = 19;
	ConsumIcon19.iTextNum = 0;
	ConsumIcon19.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon20;
	ConsumIcon20.fSizeX = 27.f;
	ConsumIcon20.fSizeY = 27.f;
	ConsumIcon20.fMoveX = 57.f;
	ConsumIcon20.fMoveY = 50.f;
	ConsumIcon20.iNum = 20;
	ConsumIcon20.iTextNum = 0;
	ConsumIcon20.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon21;
	ConsumIcon21.fSizeX = 27.f;
	ConsumIcon21.fSizeY = 27.f;
	ConsumIcon21.fMoveX = -69.f;
	ConsumIcon21.fMoveY = 92.f;
	ConsumIcon21.iNum = 21;
	ConsumIcon21.iTextNum = 0;
	ConsumIcon21.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon22;
	ConsumIcon22.fSizeX = 27.f;
	ConsumIcon22.fSizeY = 27.f;
	ConsumIcon22.fMoveX = -27.f;
	ConsumIcon22.fMoveY = 92.f;
	ConsumIcon22.iNum = 22;
	ConsumIcon22.iTextNum = 0;
	ConsumIcon22.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon23;
	ConsumIcon23.fSizeX = 27.f;
	ConsumIcon23.fSizeY = 27.f;
	ConsumIcon23.fMoveX = 15.f;
	ConsumIcon23.fMoveY = 92.f;
	ConsumIcon23.iNum = 23;
	ConsumIcon23.iTextNum = 0;
	ConsumIcon23.pTag = L"DefaultInfo";

	CUI::UIINFO ConsumIcon24;
	ConsumIcon24.fSizeX = 27.f;
	ConsumIcon24.fSizeY = 27.f;
	ConsumIcon24.fMoveX = 57.f;
	ConsumIcon24.fMoveY = 92.f;
	ConsumIcon24.iNum = 24;
	ConsumIcon24.iTextNum = 0;
	ConsumIcon24.pTag = L"DefaultInfo";

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon6)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon7)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon8)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon9)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon10)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon11)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon12)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon13)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon14)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon15)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon16)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon17)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon18)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon19)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon20)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon21)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon22)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon23)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ConsumIcon"), LEVEL_STATIC, pLayerTag, &ConsumIcon24)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_StuffItemIcon(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	//x�� 42 y�� 42
	CUI::UIINFO StuffIcon1;
	StuffIcon1.fSizeX = 27.f;
	StuffIcon1.fSizeY = 27.f;
	StuffIcon1.fMoveX = -69.f;
	StuffIcon1.fMoveY = -118.f;
	StuffIcon1.iNum = 1;
	StuffIcon1.iTextNum = 0;
	StuffIcon1.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon2;
	StuffIcon2.fSizeX = 27.f;
	StuffIcon2.fSizeY = 27.f;
	StuffIcon2.fMoveX = -27.f;
	StuffIcon2.fMoveY = -118.f;
	StuffIcon2.iNum = 2;
	StuffIcon2.iTextNum = 0;
	StuffIcon2.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon3;
	StuffIcon3.fSizeX = 27.f;
	StuffIcon3.fSizeY = 27.f;
	StuffIcon3.fMoveX = 15.f;
	StuffIcon3.fMoveY = -118.f;
	StuffIcon3.iNum = 3;
	StuffIcon3.iTextNum = 0;
	StuffIcon3.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon4;
	StuffIcon4.fSizeX = 27.f;
	StuffIcon4.fSizeY = 27.f;
	StuffIcon4.fMoveX = 57.f;
	StuffIcon4.fMoveY = -118.f;
	StuffIcon4.iNum = 4;
	StuffIcon4.iTextNum = 0;
	StuffIcon4.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon5;
	StuffIcon5.fSizeX = 27.f;
	StuffIcon5.fSizeY = 27.f;
	StuffIcon5.fMoveX = -69.f;
	StuffIcon5.fMoveY = -76.f;
	StuffIcon5.iNum = 5;
	StuffIcon5.iTextNum = 0;
	StuffIcon5.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon6;
	StuffIcon6.fSizeX = 27.f;
	StuffIcon6.fSizeY = 27.f;
	StuffIcon6.fMoveX = -27.f;
	StuffIcon6.fMoveY = -76.f;
	StuffIcon6.iNum = 6;
	StuffIcon6.iTextNum = 0;
	StuffIcon6.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon7;
	StuffIcon7.fSizeX = 27.f;
	StuffIcon7.fSizeY = 27.f;
	StuffIcon7.fMoveX = 15.f;
	StuffIcon7.fMoveY = -76.f;
	StuffIcon7.iNum = 7;
	StuffIcon7.iTextNum = 0;
	StuffIcon7.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon8;
	StuffIcon8.fSizeX = 27.f;
	StuffIcon8.fSizeY = 27.f;
	StuffIcon8.fMoveX = 57.f;
	StuffIcon8.fMoveY = -76.f;
	StuffIcon8.iNum = 8;
	StuffIcon8.iTextNum = 0;
	StuffIcon8.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon9;
	StuffIcon9.fSizeX = 27.f;
	StuffIcon9.fSizeY = 27.f;
	StuffIcon9.fMoveX = -69.f;
	StuffIcon9.fMoveY = -34.f;
	StuffIcon9.iNum = 9;
	StuffIcon9.iTextNum = 0;
	StuffIcon9.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon10;
	StuffIcon10.fSizeX = 27.f;
	StuffIcon10.fSizeY = 27.f;
	StuffIcon10.fMoveX = -27.f;
	StuffIcon10.fMoveY = -34.f;
	StuffIcon10.iNum = 10;
	StuffIcon10.iTextNum = 0;
	StuffIcon10.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon11;
	StuffIcon11.fSizeX = 27.f;
	StuffIcon11.fSizeY = 27.f;
	StuffIcon11.fMoveX = 15.f;
	StuffIcon11.fMoveY = -34.f;
	StuffIcon11.iNum = 11;
	StuffIcon11.iTextNum = 0;
	StuffIcon11.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon12;
	StuffIcon12.fSizeX = 27.f;
	StuffIcon12.fSizeY = 27.f;
	StuffIcon12.fMoveX = 57.f;
	StuffIcon12.fMoveY = -34.f;
	StuffIcon12.iNum = 12;
	StuffIcon12.iTextNum = 0;
	StuffIcon12.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon13;
	StuffIcon13.fSizeX = 27.f;
	StuffIcon13.fSizeY = 27.f;
	StuffIcon13.fMoveX = -69.f;
	StuffIcon13.fMoveY = 8.f;
	StuffIcon13.iNum = 13;
	StuffIcon13.iTextNum = 0;
	StuffIcon13.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon14;
	StuffIcon14.fSizeX = 27.f;
	StuffIcon14.fSizeY = 27.f;
	StuffIcon14.fMoveX = -27.f;
	StuffIcon14.fMoveY = 8.f;
	StuffIcon14.iNum = 14;
	StuffIcon14.iTextNum = 0;
	StuffIcon14.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon15;
	StuffIcon15.fSizeX = 27.f;
	StuffIcon15.fSizeY = 27.f;
	StuffIcon15.fMoveX = 15.f;
	StuffIcon15.fMoveY = 8.f;
	StuffIcon15.iNum = 15;
	StuffIcon15.iTextNum = 0;
	StuffIcon15.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon16;
	StuffIcon16.fSizeX = 27.f;
	StuffIcon16.fSizeY = 27.f;
	StuffIcon16.fMoveX = 57.f;
	StuffIcon16.fMoveY = 8.f;
	StuffIcon16.iNum = 16;
	StuffIcon16.iTextNum = 0;
	StuffIcon16.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon17;
	StuffIcon17.fSizeX = 27.f;
	StuffIcon17.fSizeY = 27.f;
	StuffIcon17.fMoveX = -69.f;
	StuffIcon17.fMoveY = 50.f;
	StuffIcon17.iNum = 17;
	StuffIcon17.iTextNum = 0;
	StuffIcon17.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon18;
	StuffIcon18.fSizeX = 27.f;
	StuffIcon18.fSizeY = 27.f;
	StuffIcon18.fMoveX = -27.f;
	StuffIcon18.fMoveY = 50.f;
	StuffIcon18.iNum = 18;
	StuffIcon18.iTextNum = 0;
	StuffIcon18.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon19;
	StuffIcon19.fSizeX = 27.f;
	StuffIcon19.fSizeY = 27.f;
	StuffIcon19.fMoveX = 15.f;
	StuffIcon19.fMoveY = 50.f;
	StuffIcon19.iNum = 19;
	StuffIcon19.iTextNum = 0;
	StuffIcon19.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon20;
	StuffIcon20.fSizeX = 27.f;
	StuffIcon20.fSizeY = 27.f;
	StuffIcon20.fMoveX = 57.f;
	StuffIcon20.fMoveY = 50.f;
	StuffIcon20.iNum = 20;
	StuffIcon20.iTextNum = 0;
	StuffIcon20.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon21;
	StuffIcon21.fSizeX = 27.f;
	StuffIcon21.fSizeY = 27.f;
	StuffIcon21.fMoveX = -69.f;
	StuffIcon21.fMoveY = 92.f;
	StuffIcon21.iNum = 21;
	StuffIcon21.iTextNum = 0;
	StuffIcon21.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon22;
	StuffIcon22.fSizeX = 27.f;
	StuffIcon22.fSizeY = 27.f;
	StuffIcon22.fMoveX = -27.f;
	StuffIcon22.fMoveY = 92.f;
	StuffIcon22.iNum = 22;
	StuffIcon22.iTextNum = 0;
	StuffIcon22.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon23;
	StuffIcon23.fSizeX = 27.f;
	StuffIcon23.fSizeY = 27.f;
	StuffIcon23.fMoveX = 15.f;
	StuffIcon23.fMoveY = 92.f;
	StuffIcon23.iNum = 23;
	StuffIcon23.iTextNum = 0;
	StuffIcon23.pTag = L"DefaultInfo";

	CUI::UIINFO StuffIcon24;
	StuffIcon24.fSizeX = 27.f;
	StuffIcon24.fSizeY = 27.f;
	StuffIcon24.fMoveX = 57.f;
	StuffIcon24.fMoveY = 92.f;
	StuffIcon24.iNum = 24;
	StuffIcon24.iTextNum = 0;
	StuffIcon24.pTag = L"DefaultInfo";

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon6)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon7)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon8)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon9)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon10)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon11)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon12)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon13)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon14)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon15)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon16)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon17)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon18)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon19)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon20)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon21)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon22)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon23)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_StuffIcon"), LEVEL_STATIC, pLayerTag, &StuffIcon24)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_SkillFrameBtn(const _tchar * pLayerTag)
{
	//��ųUp Btn ���� ���� 144px ���� 40px
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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_STATIC, pLayerTag, &SkillUpBtnInfo1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_STATIC, pLayerTag, &SkillUpBtnInfo2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_STATIC, pLayerTag, &SkillUpBtnInfo3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_STATIC, pLayerTag, &SkillUpBtnInfo4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillUpBtn"), LEVEL_STATIC, pLayerTag, &SkillUpBtnInfo5)))
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

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn0"), LEVEL_STATIC, pLayerTag, &SkillGradeBtnInfo0)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn1"), LEVEL_STATIC, pLayerTag, &SkillGradeBtnInfo1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn2"), LEVEL_STATIC, pLayerTag, &SkillGradeBtnInfo2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn3"), LEVEL_STATIC, pLayerTag, &SkillGradeBtnInfo3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn4"), LEVEL_STATIC, pLayerTag, &SkillGradeBtnInfo4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SkillGradeBtn5"), LEVEL_STATIC, pLayerTag, &SkillGradeBtnInfo5)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_SkillIcon(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI::UIINFO ReefAttackInfo;
	ReefAttackInfo.fSizeX = 32.f;
	ReefAttackInfo.fSizeY = 32.f;
	ReefAttackInfo.fMoveX = -129.f;
	ReefAttackInfo.fMoveY = -69.f;

	CUI::UIINFO WarriorReefInfo;
	WarriorReefInfo.fSizeX = 32.f;
	WarriorReefInfo.fSizeY = 32.f;
	WarriorReefInfo.fMoveX = 15.f;
	WarriorReefInfo.fMoveY = -69.f;

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

	CUI::UIINFO SpearPullingInfo;
	SpearPullingInfo.fSizeX = 32.f;
	SpearPullingInfo.fSizeY = 32.f;
	SpearPullingInfo.fMoveX = -129.f;
	SpearPullingInfo.fMoveY = -69.f;

	CUI::UIINFO CardinalBlastInfo;
	CardinalBlastInfo.fSizeX = 32.f;
	CardinalBlastInfo.fSizeY = 32.f;
	CardinalBlastInfo.fMoveX = 15.f;
	CardinalBlastInfo.fMoveY = -69.f;

	CUI::UIINFO ChasingShotInfo;
	ChasingShotInfo.fSizeX = 32.f;
	ChasingShotInfo.fSizeY = 32.f;
	ChasingShotInfo.fMoveX = -129.f;
	ChasingShotInfo.fMoveY = -69.f;

	CUI::UIINFO BeastInfo;
	BeastInfo.fSizeX = 32.f;
	BeastInfo.fSizeY = 32.f;
	BeastInfo.fMoveX = -129.f;
	BeastInfo.fMoveY = -69.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ReefAttackIcon"), LEVEL_STATIC, pLayerTag, &ReefAttackInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_WarriorReefIcon"), LEVEL_STATIC, pLayerTag, &WarriorReefInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SunCrossIcon"), LEVEL_STATIC, pLayerTag, &SunCrossInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SolunaSlashIcon"), LEVEL_STATIC, pLayerTag, &SolunaSlashInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SpearPullingIcon"), LEVEL_STATIC, pLayerTag, &SpearPullingInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_CardinalBlastIcon"), LEVEL_STATIC, pLayerTag, &CardinalBlastInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ChasingShotIcon"), LEVEL_STATIC, pLayerTag, &ChasingShotInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BeastIcon"), LEVEL_STATIC, pLayerTag, &BeastInfo)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_SkillInfo()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	CSunCrossInfo* pSunCross = new CSunCrossInfo;
	CSolunaSlashInfo* pSolunaSlash = new CSolunaSlashInfo;
	CReefAttackInfo* pReefAttack = new CReefAttackInfo;
	CSpearPullingInfo* pSpearPulling = new CSpearPullingInfo;
	CWarriorReefInfo* pWarriorReef = new CWarriorReefInfo;
	CCardinalBlastInfo* pCardinalBlast = new CCardinalBlastInfo;
	CChasingShotInfo* pChasingShot = new CChasingShotInfo;
	CBeastInfo* pBeast = new CBeastInfo;
		if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("ReefAttackInfo"), CSkillManager::GRADE_BEGENNER, pReefAttack)))
		return E_FAIL;

	if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("WarriorReefInfo"), CSkillManager::GRADE_BEGENNER, pWarriorReef)))
		return E_FAIL;

	if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("SunCrossInfo"), CSkillManager::GRADE_FIRST, pSunCross)))
		return E_FAIL;
	
	if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("SolunaSlashInfo"), CSkillManager::GRADE_FIRST, pSolunaSlash)))
		return E_FAIL;

	if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("SpearPullingInfo"), CSkillManager::GRADE_SECOND, pSpearPulling)))
		return E_FAIL;

	if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("CardinalBlastInfo"), CSkillManager::GRADE_SECOND, pCardinalBlast)))
		return E_FAIL;

	if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("ChasingShotInfo"), CSkillManager::GRADE_THIRD, pChasingShot)))
		return E_FAIL;

	if (FAILED(pSkillInstance->Add_SkillInfo(TEXT("BeastInfo"), CSkillManager::GRADE_FOURTH, pBeast)))
		return E_FAIL;

	return S_OK;

	
}

HRESULT CLevel_GamePlay::Ready_QuickSlot(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//Ʋ����x 15 x���� 5 y���� 5

	CUI::UIINFO QuickSlot1;
	QuickSlot1.fSizeX = 50.f;
	QuickSlot1.fSizeY = 50.f;
	QuickSlot1.fX = 350.f;
	QuickSlot1.fY = 580.f;
	QuickSlot1.iNum = 0;

	CUI::UIINFO QuickSlot2;
	QuickSlot2.fSizeX = 50.f;
	QuickSlot2.fSizeY = 50.f;
	QuickSlot2.fX = 365.f;
	QuickSlot2.fY = 635.f;
	QuickSlot2.iNum = 1;

	CUI::UIINFO QuickSlot3;
	QuickSlot3.fSizeX = 50.f;
	QuickSlot3.fSizeY = 50.f;
	QuickSlot3.fX = 405.f;
	QuickSlot3.fY = 580.f;
	QuickSlot3.iNum = 2;

	CUI::UIINFO QuickSlot4;
	QuickSlot4.fSizeX = 50.f;
	QuickSlot4.fSizeY = 50.f;
	QuickSlot4.fX = 420.f;
	QuickSlot4.fY = 635.f;
	QuickSlot4.iNum = 3;

	CUI::UIINFO QuickSlot5;
	QuickSlot5.fSizeX = 50.f;
	QuickSlot5.fSizeY = 50.f;
	QuickSlot5.fX = 460.f;
	QuickSlot5.fY = 580.f;
	QuickSlot5.iNum = 4;

	CUI::UIINFO QuickSlot6;
	QuickSlot6.fSizeX = 50.f;
	QuickSlot6.fSizeY = 50.f;
	QuickSlot6.fX = 475.f;
	QuickSlot6.fY = 635.f;
	QuickSlot6.iNum = 5;

	CUI::UIINFO QuickSlot7;
	QuickSlot7.fSizeX = 50.f;
	QuickSlot7.fSizeY = 50.f;
	QuickSlot7.fX = 515.f;
	QuickSlot7.fY = 580.f;
	QuickSlot7.iNum = 6;

	CUI::UIINFO QuickSlot8;
	QuickSlot8.fSizeX = 50.f;
	QuickSlot8.fSizeY = 50.f;
	QuickSlot8.fX = 530.f;
	QuickSlot8.fY = 635.f;
	QuickSlot8.iNum = 7;

	CUI::UIINFO QuickSlot9;
	QuickSlot9.fSizeX = 50.f;
	QuickSlot9.fSizeY = 50.f;
	QuickSlot9.fX = 735.f;
	QuickSlot9.fY = 580.f;
	QuickSlot9.iNum = 8;

	CUI::UIINFO QuickSlot10;
	QuickSlot10.fSizeX = 50.f;
	QuickSlot10.fSizeY = 50.f;
	QuickSlot10.fX = 750.f;
	QuickSlot10.fY = 635.f;
	QuickSlot10.iNum = 9;

	CUI::UIINFO QuickSlot11;
	QuickSlot11.fSizeX = 50.f;
	QuickSlot11.fSizeY = 50.f;
	QuickSlot11.fX = 790.f;
	QuickSlot11.fY = 580.f;
	QuickSlot11.iNum = 10;

	CUI::UIINFO QuickSlot12;
	QuickSlot12.fSizeX = 50.f;
	QuickSlot12.fSizeY = 50.f;
	QuickSlot12.fX = 805.f;
	QuickSlot12.fY = 635.f;
	QuickSlot12.iNum = 11;

	CUI::UIINFO QuickSlot13;
	QuickSlot13.fSizeX = 50.f;
	QuickSlot13.fSizeY = 50.f;
	QuickSlot13.fX = 845.f;
	QuickSlot13.fY = 580.f;
	QuickSlot13.iNum = 12;

	CUI::UIINFO QuickSlot14;
	QuickSlot14.fSizeX = 50.f;
	QuickSlot14.fSizeY = 50.f;
	QuickSlot14.fX = 860.f;
	QuickSlot14.fY = 635.f;
	QuickSlot14.iNum = 13;

	CUI::UIINFO QuickSlot15;
	QuickSlot15.fSizeX = 50.f;
	QuickSlot15.fSizeY = 50.f;
	QuickSlot15.fX = 900.f;
	QuickSlot15.fY = 580.f;
	QuickSlot15.iNum = 14;

	CUI::UIINFO QuickSlot16;
	QuickSlot16.fSizeX = 50.f;
	QuickSlot16.fSizeY = 50.f;
	QuickSlot16.fX = 915.f;
	QuickSlot16.fY = 635.f;
	QuickSlot16.iNum = 15;

	CUI::UIINFO QuickSlot17;
	QuickSlot17.fSizeX = 40.f;
	QuickSlot17.fSizeY = 40.f;
	QuickSlot17.fX = 945.f;
	QuickSlot17.fY = 544.f;
	QuickSlot17.iNum = 16;

	CUI::UIINFO QuickSlot18;
	QuickSlot18.fSizeX = 40.f;
	QuickSlot18.fSizeY = 40.f;
	QuickSlot18.fX = 945.f;
	QuickSlot18.fY = 584.f;
	QuickSlot18.iNum = 17;

	CUI::UIINFO QuickSlot19;
	QuickSlot19.fSizeX = 40.f;
	QuickSlot19.fSizeY = 40.f;
	QuickSlot19.fX = 985.f;
	QuickSlot19.fY = 544.f;
	QuickSlot19.iNum = 18;

	CUI::UIINFO QuickSlot20;
	QuickSlot20.fSizeX = 40.f;
	QuickSlot20.fSizeY = 40.f;
	QuickSlot20.fX = 985.f;
	QuickSlot20.fY = 584.f;
	QuickSlot20.iNum = 19;

	CUI::UIINFO QuickSlot21;
	QuickSlot21.fSizeX = 40.f;
	QuickSlot21.fSizeY = 40.f;
	QuickSlot21.fX = 1025.f;
	QuickSlot21.fY = 544.f;
	QuickSlot21.iNum = 20;

	CUI::UIINFO QuickSlot22;
	QuickSlot22.fSizeX = 40.f;
	QuickSlot22.fSizeY = 40.f;
	QuickSlot22.fX = 1025.f;
	QuickSlot22.fY = 584.f;
	QuickSlot22.iNum = 21;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot6)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot7)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot8)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot9)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot10)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot11)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot12)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot13)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot14)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot15)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot16)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot17)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot18)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot19)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot20)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot21)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_STATIC, pLayerTag, &QuickSlot22)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_QuickSlotSkill(const _tchar * pLayerTag)
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
	QuickSlotSkill2.iNum = 1;

	CUI::UIINFO QuickSlotSkill3;
	QuickSlotSkill3.fSizeX = 40.f;
	QuickSlotSkill3.fSizeY = 40.f;
	QuickSlotSkill3.fX = 405.f;
	QuickSlotSkill3.fY = 580.f;
	QuickSlotSkill3.iNum = 2;

	CUI::UIINFO QuickSlotSkill4;
	QuickSlotSkill4.fSizeX = 40.f;
	QuickSlotSkill4.fSizeY = 40.f;
	QuickSlotSkill4.fX = 420.f;
	QuickSlotSkill4.fY = 635.f;
	QuickSlotSkill4.iNum = 3;

	CUI::UIINFO QuickSlotSkill5;
	QuickSlotSkill5.fSizeX = 40.f;
	QuickSlotSkill5.fSizeY = 40.f;
	QuickSlotSkill5.fX = 460.f;
	QuickSlotSkill5.fY = 580.f;
	QuickSlotSkill5.iNum = 4;

	CUI::UIINFO QuickSlotSkill6;
	QuickSlotSkill6.fSizeX = 40.f;
	QuickSlotSkill6.fSizeY = 40.f;
	QuickSlotSkill6.fX = 475.f;
	QuickSlotSkill6.fY = 635.f;
	QuickSlotSkill6.iNum = 5;

	CUI::UIINFO QuickSlotSkill7;
	QuickSlotSkill7.fSizeX = 40.f;
	QuickSlotSkill7.fSizeY = 40.f;
	QuickSlotSkill7.fX = 515.f;
	QuickSlotSkill7.fY = 580.f;
	QuickSlotSkill7.iNum = 6;

	CUI::UIINFO QuickSlotSkill8;
	QuickSlotSkill8.fSizeX = 40.f;
	QuickSlotSkill8.fSizeY = 40.f;
	QuickSlotSkill8.fX = 530.f;
	QuickSlotSkill8.fY = 635.f;
	QuickSlotSkill8.iNum = 7;

	CUI::UIINFO QuickSlotSkill9;
	QuickSlotSkill9.fSizeX = 40.f;
	QuickSlotSkill9.fSizeY = 40.f;
	QuickSlotSkill9.fX = 735.f;
	QuickSlotSkill9.fY = 580.f;
	QuickSlotSkill9.iNum = 8;
	
	CUI::UIINFO QuickSlotSkill10;
	QuickSlotSkill10.fSizeX = 40.f;
	QuickSlotSkill10.fSizeY = 40.f;
	QuickSlotSkill10.fX = 750.f;
	QuickSlotSkill10.fY = 635.f;
	QuickSlotSkill10.iNum = 9;

	CUI::UIINFO QuickSlotSkill11;
	QuickSlotSkill11.fSizeX = 40.f;
	QuickSlotSkill11.fSizeY = 40.f;
	QuickSlotSkill11.fX = 790.f;
	QuickSlotSkill11.fY = 580.f;
	QuickSlotSkill11.iNum = 10;
	
	CUI::UIINFO QuickSlotSkill12;
	QuickSlotSkill12.fSizeX = 40.f;
	QuickSlotSkill12.fSizeY = 40.f;
	QuickSlotSkill12.fX = 805.f;
	QuickSlotSkill12.fY = 635.f;
	QuickSlotSkill12.iNum = 11;

	CUI::UIINFO QuickSlotSkill13;
	QuickSlotSkill13.fSizeX = 40.f;
	QuickSlotSkill13.fSizeY = 40.f;
	QuickSlotSkill13.fX = 845.f;
	QuickSlotSkill13.fY = 580.f;
	QuickSlotSkill13.iNum = 12;

	CUI::UIINFO QuickSlotSkill14;
	QuickSlotSkill14.fSizeX = 40.f;
	QuickSlotSkill14.fSizeY = 40.f;
	QuickSlotSkill14.fX = 860.f;
	QuickSlotSkill14.fY = 635.f;
	QuickSlotSkill14.iNum = 13;

	CUI::UIINFO QuickSlotSkill15;
	QuickSlotSkill15.fSizeX = 40.f;
	QuickSlotSkill15.fSizeY = 40.f;
	QuickSlotSkill15.fX = 900.f;
	QuickSlotSkill15.fY = 580.f;
	QuickSlotSkill15.iNum = 14;

	CUI::UIINFO QuickSlotSkill16;
	QuickSlotSkill16.fSizeX = 40.f;
	QuickSlotSkill16.fSizeY = 40.f;
	QuickSlotSkill16.fX = 915.f;
	QuickSlotSkill16.fY = 635.f;
	QuickSlotSkill16.iNum = 15;

	


	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill6)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill7)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill8)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill9)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill10)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill11)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill12)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill13)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill14)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill15)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotSkill"), LEVEL_STATIC, pLayerTag, &QuickSlotSkill16)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_QuickSlotItem(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CUI::UIINFO QuickSlotItem1;
	QuickSlotItem1.fSizeX = 27.f;
	QuickSlotItem1.fSizeY = 27.f;
	QuickSlotItem1.fX = 945.f;
	QuickSlotItem1.fY = 544.f;
	QuickSlotItem1.iNum = 16;

	CUI::UIINFO QuickSlotItem2;
	QuickSlotItem2.fSizeX = 27.f;
	QuickSlotItem2.fSizeY = 27.f;
	QuickSlotItem2.fX = 945.f;
	QuickSlotItem2.fY = 584.f;
	QuickSlotItem2.iNum = 17;

	CUI::UIINFO QuickSlotItem3;
	QuickSlotItem3.fSizeX = 27.f;
	QuickSlotItem3.fSizeY = 27.f;
	QuickSlotItem3.fX = 985.f;
	QuickSlotItem3.fY = 544.f;
	QuickSlotItem3.iNum = 18;

	CUI::UIINFO QuickSlotItem4;
	QuickSlotItem4.fSizeX = 27.f;
	QuickSlotItem4.fSizeY = 27.f;
	QuickSlotItem4.fX = 985.f;
	QuickSlotItem4.fY = 584.f;
	QuickSlotItem4.iNum = 19;

	CUI::UIINFO QuickSlotItem5;
	QuickSlotItem5.fSizeX = 27.f;
	QuickSlotItem5.fSizeY = 27.f;
	QuickSlotItem5.fX = 1025.f;
	QuickSlotItem5.fY = 544.f;
	QuickSlotItem5.iNum = 20;

	CUI::UIINFO QuickSlotItem6;
	QuickSlotItem6.fSizeX = 27.f;
	QuickSlotItem6.fSizeY = 27.f;
	QuickSlotItem6.fX = 1025.f;
	QuickSlotItem6.fY = 584.f;
	QuickSlotItem6.iNum = 21;
	
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotItem"), LEVEL_STATIC, pLayerTag, &QuickSlotItem1)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotItem"), LEVEL_STATIC, pLayerTag, &QuickSlotItem2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotItem"), LEVEL_STATIC, pLayerTag, &QuickSlotItem3)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotItem"), LEVEL_STATIC, pLayerTag, &QuickSlotItem4)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotItem"), LEVEL_STATIC, pLayerTag, &QuickSlotItem5)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_QuickSlotItem"), LEVEL_STATIC, pLayerTag, &QuickSlotItem6)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}



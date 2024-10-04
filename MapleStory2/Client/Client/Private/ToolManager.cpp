#include "stdafx.h"
#include "..\Public\ToolManager.h"

#include "GameInstance.h"
#include "Player.h"
#include "Level_Loading.h"
#include "SpawnerManager.h"


IMPLEMENT_SINGLETON(CToolManager)

CToolManager::CToolManager()
{

}




void CToolManager::SetDestLevel(LEVEL eLevel, _float3 vDestPos)
{

		m_eDestLevel = eLevel; m_vDestPos = vDestPos;	
}

void CToolManager::CheckDestLevel(CCollider* pColliderCom, LPDIRECT3DDEVICE9 pGraphic_Device)
{
	if (LEVEL_END == m_eDestLevel)
		return;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_STATIC, TEXT("Layer_Player"), 0);;
	pPlayer->SetRespownPos(m_vDestPos);

	pColliderCom->ResetSection();


	if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(pGraphic_Device, m_eDestLevel))))
		return;

	m_eDestLevel = LEVEL_END;


	CSpawnerManager::Get_Instance()->Free_Spawner();

	Safe_Release(pGameInstance);

}

LEVEL CToolManager::Get_CurLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	_uint iLevel = pGameInstance->Get_CurLevel();


	Safe_Release(pGameInstance);

	return (LEVEL)iLevel;
}

CGameObject* CToolManager::GetPlayer()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pObj = pGameInstance->Get_ObjectPtr(LEVEL_STATIC, TEXT("Layer_Player"), 0);

	Safe_Release(pGameInstance);

	return pObj;
}

void CToolManager::MakeMiniBlock(_float3 vPos, CModel_MiniBlock::MINICOLOR eType)
{
	CModel_MiniBlock::MINIBLOCKDESC pVoxDesc;
	vPos.y += 2.f;
	pVoxDesc.vPos = vPos;
	pVoxDesc.etype = eType;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Model_MiniBlock"), LEVEL_DHENESYSHUNTING, TEXT("Layer_Model"), &pVoxDesc)))
		return;
}

void CToolManager::MakeMiniDownCube()
{

	_float4x4 RotationMatrix;
	D3DXMatrixIdentity(&RotationMatrix);
	D3DXMatrixRotationAxis(&RotationMatrix, &_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(45.f));

	_float3 vPos;
	vPos.y = 0.f;

	for (_int z = 0; z < 5; z++)
	{
		for (_int x = 0; x < 5; x++)
		{
			vPos.x = (_float)x - 2.f;
			vPos.z = (_float)z - 17.f;
			vPos.y = 0.f;

			D3DXVec3TransformNormal(&vPos, &vPos, &RotationMatrix);

			CMap_Manager::CUBEDATA pCubeDesc;
			pCubeDesc.vPos = vPos;
			pCubeDesc.iIndex = 17;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_DownCube"), LEVEL_DHENESYSHUNTING, TEXT("Layer_Cube"), &pCubeDesc)))
				return;

		}
	}


	_float3 vTemp;

	vTemp = _float3{ 2.f, 0.f, -2.f - 15.f };
	vPos.y = 2.f;
	D3DXVec3TransformNormal(&vTemp, &vTemp, &RotationMatrix);
	MakeMiniBlock(vTemp, CModel_MiniBlock::MINI_RED);
	vTemp = _float3{ 0.f, 0.f, 0.f - 15.f };
	vPos.y = 2.f;
	D3DXVec3TransformNormal(&vTemp, &vTemp, &RotationMatrix);
	MakeMiniBlock(vTemp, CModel_MiniBlock::MINI_RED);

	vTemp = _float3{ 1.f, 0.f, -1.f - 15.f };
	vPos.y = 2.f;
	D3DXVec3TransformNormal(&vTemp, &vTemp, &RotationMatrix);
	MakeMiniBlock(vTemp, CModel_MiniBlock::MINI_ORRANGE);
	vTemp = _float3{ -1.f, 0.f, -1.f - 15.f };
	vPos.y = 2.f;
	D3DXVec3TransformNormal(&vTemp, &vTemp, &RotationMatrix);
	MakeMiniBlock(vTemp, CModel_MiniBlock::MINI_ORRANGE);

	vTemp = _float3{ -1.f, 0.f, -2.f - 15.f };
	vPos.y = 2.f;
	D3DXVec3TransformNormal(&vTemp, &vTemp, &RotationMatrix);
	MakeMiniBlock(vTemp, CModel_MiniBlock::MINI_YERROW);
	vTemp = _float3{ 2.f, 0.f, -1.f - 15.f };
	vPos.y = 2.f;
	D3DXVec3TransformNormal(&vTemp, &vTemp, &RotationMatrix);
	MakeMiniBlock(vTemp, CModel_MiniBlock::MINI_YERROW);


	m_ePickedMiniBlock = CModel_MiniBlock::MINI_END;

	CGameInstance::Get_Instance()->PlaySoundW(L"OpenMini.mp3", 4, 1.f);
}




void CToolManager::Free()
{
	
}



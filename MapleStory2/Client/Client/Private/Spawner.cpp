#include "stdafx.h"
#include "..\Public\Spawner.h"
#include "GameInstance.h"
#include "SpawnerManager.h"

CSpawner::CSpawner(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	//ZeroMemory(&m_SpawnerInfo, sizeof(m_SpawnerInfo));

}

CSpawner::CSpawner(const CSpawner & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSpawner::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CSpawner::Initialize(void * pArg)
{

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fCount = 0;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_SpawnerInfo = *(CSpawner::SPAWNERINFO*)pArg;

	m_iMonsterNow = m_SpawnerInfo.MonsterNum;
	Set_Index(m_SpawnerInfo.SpawnerNum);

	// monsternum값 만큼 생성시키면됨
	for (int i = 0; i < m_SpawnerInfo.MonsterNum; ++i)
	{
		m_SpawnerInfo.MonsterPos -= _float3{ _float(i) * 0.2f, 0.f, 0.f };

		if (m_SpawnerInfo.MonsterName == TEXT("OrangeMushroom"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_OrangeMushroom"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("RibbonPig"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_RibbonPig"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}
		
		else if (m_SpawnerInfo.MonsterName == TEXT("RedSnail"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_RedSnail"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("BlueSnail"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlueSnail"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("GreenMushroom"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GreenMushroom"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("GoStump"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GoStump"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("Slime"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Slime"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("TransformPig"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TransformPig"), LEVEL_DHENESYSHUNTING, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("TransformStump"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TransformStump"), LEVEL_DHENESYSHUNTING, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

		else if (m_SpawnerInfo.MonsterName == TEXT("TransformSlime"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TransformSlime"), LEVEL_DELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
				return E_FAIL;
		}

	
 	}

	CSpawnerManager::Get_Instance()->Add_SpawnerInfo(this);




	Safe_Release(pGameInstance);


	

	return S_OK;
}

void CSpawner::Tick(_float fTimeDelta)
{

}

void CSpawner::LateTick(_float fTimeDelta)
{
	m_fCount += 1.f * fTimeDelta;

	if (m_SpawnerInfo.MonsterNum > m_iMonsterNow)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		int Num = m_SpawnerInfo.MonsterNum - m_iMonsterNow;

		if (5.f < m_fCount)
		{
			m_fCount = 0;

			for (int i = 0; i < Num; ++i)
			{
				_float Random = pGameInstance->Get_Random(0, 4);
				if (Random == 1)
					m_SpawnerInfo.MonsterPos += { 0.3f, 0.f, 0.5f};
				else if (Random == 2)
					m_SpawnerInfo.MonsterPos += {-0.3f, 0.f, 0.5f};
				else if (Random == 3)
					m_SpawnerInfo.MonsterPos += { 0.3f, 0.f, -0.5f};
				else if (Random == 4)
					m_SpawnerInfo.MonsterPos += {-0.3f, 0.f, -0.5f};

				if (m_SpawnerInfo.MonsterName == TEXT("OrangeMushroom"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_OrangeMushroom"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("RibbonPig"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_RibbonPig"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("RedSnail"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_RedSnail"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("BlueSnail"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlueSnail"), LEVEL_HENESYS, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("GreenMushroom"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GreenMushroom"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("GoStump"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GoStump"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("Slime"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Slime"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("TransformPig"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TransformPig"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("TransformSlime"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TransformSlime"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				else if (m_SpawnerInfo.MonsterName == TEXT("TransformStump"))
				{
					if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TransformStump"), LEVEL_ELENYA, TEXT("Layer_Monster"), &m_SpawnerInfo)))
						int a = 0;
				}

				Plus_Mushroom();
			}
		}

		
		Safe_Release(pGameInstance);
	}

}

HRESULT CSpawner::Render()
{


	return S_OK;
}

HRESULT CSpawner::SetUp_Components()
{

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;




	
	return S_OK;
}


CSpawner * CSpawner::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpawner*		pInstance = new CSpawner(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSpawner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpawner::Clone(void* pArg)
{
	CSpawner*		pInstance = new CSpawner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CSpawner clone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawner::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}


#include "stdafx.h"
#include "..\Public\DamageGen.h"

#include "GameInstance.h"
#include "ToolManager.h"


CDamageGen::CDamageGen(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CDamageGen::CDamageGen(const CDamageGen & rhs)
	: CCreature(rhs)
{
}




HRESULT CDamageGen::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CDamageGen::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_Desc = *(DAMAGEGENDESC*)pArg;

	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}




HRESULT CDamageGen::SetUp_Components()
{



	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 4.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}





void CDamageGen::Tick(_float fTimeDelta)
{

	if (m_Desc.bIm)
		Im(fTimeDelta);
	else
		NonIm(fTimeDelta);

}
void CDamageGen::LateTick(_float fTimeDelta)
{

}
HRESULT CDamageGen::Render()
{

	if (FAILED(Set_RenderState()))
		return E_FAIL;



	if (FAILED(Reset_RenderState()))
		return E_FAIL;



	return S_OK;
}





CDamageGen * CDamageGen::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDamageGen*		pInstance = new CDamageGen(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CDamage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CDamageGen::Clone(void* pArg)
{
	CDamageGen*		pInstance = new CDamageGen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CDamage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CDamageGen::Collision(CGameObject * pOther)
{

}

void CDamageGen::Im(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	LEVEL eCurLevel = CToolManager::Get_Instance()->Get_CurLevel();
	_float3 vTempPos = m_Desc.vPos;

	for (_int i = 0; i < m_Desc.iCount; i++)
	{

		_int iDamage = pGameInstance->Get_Random(m_Desc.sMinDamage, m_Desc.sMaxDamage);
		_float iX = pGameInstance->Get_FloatRandom(-.2f, .2f);


		CDamage::DAMAGEDESC DamageDesc;
		DamageDesc.sDamage = std::to_string(iDamage);
		vTempPos.y = m_Desc.vPos.y + 0.5f * i;
		vTempPos.x += iX;
		DamageDesc.vPos = vTempPos;
		DamageDesc.fLifeTime = m_Desc.fLifeTime;
		DamageDesc.eType = m_Desc.eType;

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Damage"), eCurLevel, TEXT("Layer_Damage"), &DamageDesc);

	}

	if (CDamage::DAMAGE_HEAL != m_Desc.eType)
	{
		CGameInstance::Get_Instance()->PlaySound(L"SnailDamage.wav", 5, 1.f);
	}
	
	
	Set_Dead();

	Safe_Release(pGameInstance);
}

void CDamageGen::NonIm(_float fTimeDelta)
{

	m_fTimeAcc += fTimeDelta;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	LEVEL eCurLevel = CToolManager::Get_Instance()->Get_CurLevel();
	_float3 vTempPos = m_Desc.vPos;

	if (m_Desc.fDelayTime < m_fTimeAcc)
	{
		if (m_Desc.iCount < m_iCount)
			Set_Dead();


		_int iDamage = pGameInstance->Get_Random(m_Desc.sMinDamage, m_Desc.sMaxDamage);

		_float iX = pGameInstance->Get_FloatRandom(-.2f, .2f);

		CDamage::DAMAGEDESC DamageDesc;
		DamageDesc.sDamage = std::to_string(iDamage);
		vTempPos.y = m_Desc.vPos.y + 0.5f * m_iCount;
		vTempPos.x += iX;
		DamageDesc.vPos = vTempPos;
		DamageDesc.fLifeTime = m_Desc.fLifeTime;
		DamageDesc.eType = m_Desc.eType;

		if (CDamage::DAMAGE_HEAL != m_Desc.eType)
			CGameInstance::Get_Instance()->PlaySound(L"SnailDamage.wav", 4, 1.f);

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Damage"), eCurLevel, TEXT("Layer_Damage"), &DamageDesc);


		m_fTimeAcc = 0.f;
		++m_iCount;
	}

	Safe_Release(pGameInstance);
}



void CDamageGen::Free()
{
	__super::Free();
}


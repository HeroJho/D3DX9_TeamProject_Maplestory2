#include "stdafx.h"
#include "..\Public\Damage.h"

#include "GameInstance.h"


CDamage::CDamage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CDamage::CDamage(const CDamage & rhs)
	: CCreature(rhs)
{
}




HRESULT CDamage::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CDamage::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_Desc = *(DAMAGEDESC*)pArg;

	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fScale = 1.0f;
	m_fInitScale = 2.f;
	m_fMddleScale = 0.5f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Desc.vPos);
	m_pTransformCom->Set_Scaled(m_fInitScale);

	return S_OK;
}




HRESULT CDamage::SetUp_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Damage"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 4.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamage::SetDamage(_char cNum)
{
	_int iTypeNum = 0;
	switch (m_Desc.eType)
	{
	case DAMAGE_ATTACKED:
		iTypeNum += 0;
		break;
	case DAMAGE_PLAYERATTAKED:
		iTypeNum += 21;
		break;
	case DAMAGE_HEAL:
		iTypeNum += 11;
		break;
	}



	switch (cNum)
	{
	case '0':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum)))
			return E_FAIL;
		break;
	case '1':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 1)))
			return E_FAIL;
		break;
	case '2':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 2)))
			return E_FAIL;
		break;
	case '3':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 3)))
			return E_FAIL;
		break;
	case '4':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 4)))
			return E_FAIL;
		break;
	case '5':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 5)))
			return E_FAIL;
		break;
	case '6':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 6)))
			return E_FAIL;
		break;
	case '7':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 7)))
			return E_FAIL;
		break;
	case '8':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 8)))
			return E_FAIL;
		break;
	case '9':
		if (FAILED(m_pTextureCom->Bind_Texture(iTypeNum + 9)))
			return E_FAIL;
		break;
	}

	return S_OK;
}




void CDamage::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_Desc.fLifeTime < m_fTimeAcc)
	{
		Set_Dead();

		m_fTimeAcc = 0.f;
	}


	if (m_bEnd)
		return;

	m_fTimeScaleAcc += fTimeDelta;
	m_fScaleAcc += 0.01f;

	if (0.05f < m_fTimeScaleAcc)
	{
		if (m_fMddleScale < m_fCulScale && !m_bDown)
		{
			_float MinusNum = 1.f - m_fScaleAcc;
			m_fCulScale = m_fInitScale - MinusNum;
			m_fScaleAcc = 0.f;
			m_bDown = true;
		}
		else if (m_fScale > m_fCulScale)
		{
			m_fCulScale += 0.3f;
		}
		else
		{
			m_bEnd = true;
			m_fCulScale = m_fScale;
		}
			
		m_pTransformCom->Set_Scaled(m_fCulScale);
		m_fTimeScaleAcc = 0.f;
	}

}
void CDamage::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}
HRESULT CDamage::Render()
{
	Set_Billboard();


	if (FAILED(Set_RenderState()))
		return E_FAIL;

    _float3 vOriPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vPos = vOriPos;

	for (_int i = 0; i < m_Desc.sDamage.size(); i++)
	{
		vPos.x = vOriPos.x + m_fScale * i * 0.4f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(SetDamage(m_Desc.sDamage[i])))
			return E_FAIL;

		m_pVIBufferCom->Render();
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOriPos);


	if (FAILED(Reset_RenderState()))
		return E_FAIL;



	return S_OK;
}





CDamage * CDamage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDamage*		pInstance = new CDamage(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CDamage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CDamage::Clone(void* pArg)
{
	CDamage*		pInstance = new CDamage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CDamage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CDamage::Collision(CGameObject * pOther)
{

}



void CDamage::Free()
{
	__super::Free();
}


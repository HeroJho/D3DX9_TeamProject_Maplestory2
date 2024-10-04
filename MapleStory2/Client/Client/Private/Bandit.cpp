#include "stdafx.h"
#include "..\Public\Bandit.h"

#include "GameInstance.h"
#include "Player.h"
#include "Level_Loading.h"

CBandit::CBandit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBandit::CBandit(const CBandit & rhs)
	: CCreature(rhs)
{
}




HRESULT CBandit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBandit::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;


	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	BANDITDESC* pDesc = (BANDITDESC*)pArg;


	m_sTag = "Tag_Bandit";

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
	m_pTransformCom->Set_Scaled(pDesc->fScale);
	m_fOriScale = pDesc->fScale;
	m_fCurScale = m_fOriScale;
	m_fTime = pDesc->fTime;

	return S_OK;
}




HRESULT CBandit::SetUp_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_ELENYA, TEXT("Prototype_Component_Texture_Bandit"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBandit::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CBandit::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;
}




void CBandit::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTime < m_fTimeAcc)
	{
		if (0.1f >= m_fCurScale)
			m_bSwitch = true;
		else if (m_fOriScale <= m_fCurScale)
			m_bSwitch = false;

		if (m_bSwitch)
			m_fCurScale += 0.1f;
		else
			m_fCurScale -= 0.1f;

		m_pTransformCom->Set_Scaled(m_fCurScale);

		m_fTimeAcc = 0.f;
	}

}
void CBandit::LateTick(_float fTimeDelta)
{
	__super::Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}
HRESULT CBandit::Render()
{
	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;


	return S_OK;
}





CBandit * CBandit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBandit*		pInstance = new CBandit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBandit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBandit::Clone(void* pArg)
{
	CBandit*		pInstance = new CBandit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBandit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBandit::Collision(CGameObject * pOther)
{

}




void CBandit::Free()
{
	__super::Free();

}


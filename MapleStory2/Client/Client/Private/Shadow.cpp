#include "stdafx.h"
#include "..\Public\Shadow.h"

#include "GameInstance.h"



CShadow::CShadow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CShadow::CShadow(const CShadow & rhs)
	: CCreature(rhs)
{
}




HRESULT CShadow::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CShadow::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SHADOWDESC* pDesc = (SHADOWDESC*)pArg;

	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Shadow";
	m_fOriScale = pDesc->fScale * 0.4f;

	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-10.f, 2.f, -2.f));
	m_pTransformCom->Rotation(_float3{ 1.f, 0.f, 0.f }, 90.f);
	m_pTransformCom->Set_Scaled(m_fOriScale);

	return S_OK;
}




HRESULT CShadow::SetUp_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shadow"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CShadow::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CShadow::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CShadow::Tick(_float fTimeDelta)
{



}
void CShadow::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_MOVEALPHABLEND, this);
}
HRESULT CShadow::Render()
{

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





CShadow * CShadow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CShadow*		pInstance = new CShadow(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CShadow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CShadow::Clone(void* pArg)
{
	CShadow*		pInstance = new CShadow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CShadow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CShadow::Collision(CGameObject * pOther)
{

}




void CShadow::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MushHouse.h"

#include "GameInstance.h"

CMushHouse::CMushHouse(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CMushHouse::CMushHouse(const CMushHouse & rhs)
	: CGameObject(rhs)
{
}


HRESULT CMushHouse::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMushHouse::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	CVIBuffer_Voxel::VOXELDESC* pVoxDesc = (CVIBuffer_Voxel::VOXELDESC*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pVoxDesc->vPos);
	m_pTransformCom->Set_Scaled(pVoxDesc->vScale);



	return S_OK;
}

void CMushHouse::Tick(_float fTimeDelta)
{

}

void CMushHouse::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMushHouse::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMushHouse::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	return S_OK;
}

HRESULT CMushHouse::Reset_RenderState()
{


	return S_OK;
}

HRESULT CMushHouse::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_MushHouse"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
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

CMushHouse * CMushHouse::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMushHouse*		pInstance = new CMushHouse(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMushHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMushHouse::Clone(void* pArg)
{
	CMushHouse*		pInstance = new CMushHouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMushHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMushHouse::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


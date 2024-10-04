#include "stdafx.h"
#include "..\Public\Wood.h"

#include "GameInstance.h"

CWood::CWood(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CWood::CWood(const CWood & rhs)
	: CGameObject(rhs)
{
}


HRESULT CWood::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWood::Initialize(void * pArg)
{
	CVIBuffer_Voxel::VOXELDESC* pVoxDesc = (CVIBuffer_Voxel::VOXELDESC*)pArg;



	if (FAILED(SetUp_Components(pVoxDesc->cFileName)))
		return E_FAIL;	

	if (nullptr == pArg)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pVoxDesc->vPos);
	m_pTransformCom->Set_Scaled(pVoxDesc->vScale);
	m_pTransformCom->RotationThree(
		_float3(1.f, 0.f, 0.f), pVoxDesc->vRotationAix.x,
		_float3(0.f, 1.f, 0.f), pVoxDesc->vRotationAix.y + 45.f,
		_float3(0.f, 0.f, 1.f), pVoxDesc->vRotationAix.z);

	return S_OK;
}

void CWood::Tick(_float fTimeDelta)
{
	
}

void CWood::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CWood::Render()
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

HRESULT CWood::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	return S_OK;
}

HRESULT CWood::Reset_RenderState()
{


	return S_OK;
}

HRESULT CWood::SetUp_Components(_tchar* pName)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */



	if (FAILED(__super::Add_Component(LEVEL_STATIC, pName, TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
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

CWood * CWood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWood*		pInstance = new CWood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWood::Clone(void* pArg)
{
	CWood*		pInstance = new CWood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CWood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWood::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


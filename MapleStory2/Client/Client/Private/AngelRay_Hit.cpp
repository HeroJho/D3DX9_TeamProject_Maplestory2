#include "stdafx.h"
#include "..\Public\AngelRay_Hit.h"
#include "GameInstance.h"

CAngelRay_Hit::CAngelRay_Hit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CAngelRay_Hit::CAngelRay_Hit(const CAngelRay_Hit & rhs)
	: CCreature(rhs)
{
}




HRESULT CAngelRay_Hit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CAngelRay_Hit::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_Desc, pArg, sizeof(ANGELHITDESC));

	/*_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	_float3 vLookCamera = (*(_float3*)&ViewMatrix.m[0][0]) - m_Desc.vPos;
	D3DXVec3Normalize(&vLookCamera, &vLookCamera);
	+vLookCamera*-0.1f*/
	m_fColRad = 0.1f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Desc.vPos);
	m_pTransformCom->Set_Scaled(3.f);
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_AngelRay_Hit"), 0.08f, CAnimator::STATE_LOOF);
	m_fYDistance = 10.f;
	//m_pTransformCom->Get_State(CTransform::STATE_POSITION).y
	Set_Billboard();
	_float3 vCLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 vPo = m_Desc.vPos - m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.25f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPo);
	return S_OK;
}




HRESULT CAngelRay_Hit::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AngelRay_Hit"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 4.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CAngelRay_Hit::Tick(_float fTimeDelta)
{
	
}
void CAngelRay_Hit::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	
	if (m_pAnimatorCom->Get_AnimCount() == 5)
		Set_Dead();

	Set_Billboard();
		
}
HRESULT CAngelRay_Hit::Render()
{

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;
	//m_pTextureCom->Bind_Texture();

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;



	return S_OK;
}





void CAngelRay_Hit::Tick_Idle(_float fTimeDelta)
{

}
void CAngelRay_Hit::Tick_Move(_float fTimeDelta)
{
}
void CAngelRay_Hit::Tick_Hit(_float fTimeDelta)
{
}




void CAngelRay_Hit::SetState(STATE eState, DIR eDir)
{

}
void CAngelRay_Hit::SetAni()
{
	
}






CAngelRay_Hit * CAngelRay_Hit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAngelRay_Hit*		pInstance = new CAngelRay_Hit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CAngelRay_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CAngelRay_Hit::Clone(void* pArg)
{
	CAngelRay_Hit*		pInstance = new CAngelRay_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CAngelRay_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CAngelRay_Hit::Collision(CGameObject * pOther)
{
	
}

HRESULT CAngelRay_Hit::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CAngelRay_Hit::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pTransformCom->CulRUByLook(m_vLookTemp);
	return S_OK;
}




void CAngelRay_Hit::Free()
{
	__super::Free();
	
}


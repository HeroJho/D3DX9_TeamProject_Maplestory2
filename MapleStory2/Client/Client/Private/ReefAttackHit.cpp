#include "stdafx.h"
#include "..\Public\ReefAttackHit.h"
#include "GameInstance.h"

CReefAttackHit::CReefAttackHit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CReefAttackHit::CReefAttackHit(const CReefAttackHit & rhs)
	: CCreature(rhs)
{
}




HRESULT CReefAttackHit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CReefAttackHit::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_Desc, pArg, sizeof(REEFATTACKHITDESC));

	m_fColRad = 0.1f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Desc.vPos);
	m_pTransformCom->Set_Scaled(3.f);
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_ReefAttack_Hit"), 0.08f, CAnimator::STATE_LOOF);
	m_fYDistance = 11.f;

	Set_Billboard();
	_float3 vTest = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vPo = vTest - m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPo);
	return S_OK;

	/*
	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	_float3 vCPos = (*(_float3*)&ViewMatrix.m[3][0]);
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vCLook = vCPos - vPos;
	D3DXVec3Normalize(&vCLook, &vCLook);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + vCLook*5.f);
	*/
}



HRESULT CReefAttackHit::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ReefAttack_Hit"), nullptr);
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




void CReefAttackHit::Tick(_float fTimeDelta)
{

}
void CReefAttackHit::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_pAnimatorCom->Get_AnimCount() == 4)
		Set_Dead();


}
HRESULT CReefAttackHit::Render()
{

	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;



	return S_OK;
}



void CReefAttackHit::SetState(STATE eState, DIR eDir)
{

}
void CReefAttackHit::SetAni()
{

}






CReefAttackHit * CReefAttackHit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CReefAttackHit*		pInstance = new CReefAttackHit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CReefAttackHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CReefAttackHit::Clone(void* pArg)
{
	CReefAttackHit*		pInstance = new CReefAttackHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CReefAttackHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CReefAttackHit::Collision(CGameObject * pOther)
{

}

HRESULT CReefAttackHit::Set_RenderState()
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

HRESULT CReefAttackHit::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pTransformCom->CulRUByLook(m_vLookTemp);
	return S_OK;
}




void CReefAttackHit::Free()
{
	__super::Free();

}


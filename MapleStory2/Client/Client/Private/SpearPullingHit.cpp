#include "stdafx.h"
#include "..\Public\SpearPullingHit.h"
#include "GameInstance.h"

CSpearPullingHit::CSpearPullingHit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CSpearPullingHit::CSpearPullingHit(const CSpearPullingHit & rhs)
	: CCreature(rhs)
{
}




HRESULT CSpearPullingHit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CSpearPullingHit::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_Desc, pArg, sizeof(SPEARPULLINGHITDESC));

	m_fColRad = 0.1f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Desc.vPos);
	m_pTransformCom->Set_Scaled(3.f);
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_SpearPulling_Hit"), 0.08f, CAnimator::STATE_LOOF);
	m_fYDistance = 11.f;

	Set_Billboard();
	_float3 vTest = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vPo = vTest - m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPo);
	return S_OK;

}



HRESULT CSpearPullingHit::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SpearPulling_Hit"), nullptr);
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




void CSpearPullingHit::Tick(_float fTimeDelta)
{

}
void CSpearPullingHit::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_pAnimatorCom->Get_AnimCount() == 4)
		Set_Dead();


}
HRESULT CSpearPullingHit::Render()
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



void CSpearPullingHit::SetState(STATE eState, DIR eDir)
{

}
void CSpearPullingHit::SetAni()
{

}






CSpearPullingHit * CSpearPullingHit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpearPullingHit*		pInstance = new CSpearPullingHit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSpearPullingHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CSpearPullingHit::Clone(void* pArg)
{
	CSpearPullingHit*		pInstance = new CSpearPullingHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSpearPullingHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CSpearPullingHit::Collision(CGameObject * pOther)
{

}

HRESULT CSpearPullingHit::Set_RenderState()
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

HRESULT CSpearPullingHit::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pTransformCom->CulRUByLook(m_vLookTemp);
	return S_OK;
}




void CSpearPullingHit::Free()
{
	__super::Free();

}


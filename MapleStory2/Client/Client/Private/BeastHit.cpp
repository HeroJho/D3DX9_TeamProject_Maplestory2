#include "stdafx.h"
#include "..\Public\BeastHit.h"
#include "GameInstance.h"

CBeastHit::CBeastHit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBeastHit::CBeastHit(const CBeastHit & rhs)
	: CCreature(rhs)
{
}

HRESULT CBeastHit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBeastHit::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)pArg);
	m_pTransformCom->Set_Scaled(5.f);
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Beast_Hit"), 0.08f, CAnimator::STATE_ONCE);
	m_fYDistance = 10.f;
	Set_Billboard();
	_float3 vCLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 vPo = *(_float3*)pArg - m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.25f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPo);
	return S_OK;
}

HRESULT CBeastHit::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Beast_Hit"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}




void CBeastHit::Tick(_float fTimeDelta)
{

}
void CBeastHit::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_pAnimatorCom->Get_AnimCount() == 10)
		Set_Dead();

	Set_Billboard();

}
HRESULT CBeastHit::Render()
{

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


void CBeastHit::SetAni()
{

}


CBeastHit * CBeastHit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBeastHit*		pInstance = new CBeastHit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBeastHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBeastHit::Clone(void* pArg)
{
	CBeastHit*		pInstance = new CBeastHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBeastHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBeastHit::Collision(CGameObject * pOther)
{

}

HRESULT CBeastHit::Set_RenderState()
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

HRESULT CBeastHit::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pTransformCom->CulRUByLook(m_vLookTemp);
	return S_OK;
}




void CBeastHit::Free()
{
	__super::Free();

}


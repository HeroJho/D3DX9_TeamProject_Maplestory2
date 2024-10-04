#include "stdafx.h"
#include "..\Public\Harp.h"
#include "GameInstance.h"


CHarp::CHarp(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CHarp::CHarp(const CHarp & rhs)
	: CCreature(rhs)
{
}

HRESULT CHarp::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CHarp::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	
	CCreature::CRETUREDESC* pHarpDesc = (CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pHarpDesc->vPos);
	m_pTransformCom->Set_Scaled(2.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CHarp::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Harp"), nullptr);


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CHarp::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{

	case Client::CHarp::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CHarp::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CHarp::STATE_HIT:
		
		Tick_Hit(fTimeDelta);
		break;

	case Client::CHarp::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CHarp::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CHarp::Render()
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
void CHarp::Tick_Idle(_float fTimeDelta)
{

}
void CHarp::Tick_Stand(_float fTimeDelta)
{

}
void CHarp::Tick_Move(_float fTimeDelta)
{

}
void CHarp::Tick_Hit(_float fTimeDelta)
{

}

void CHarp::Tick_Chase(_float fTimeDelta)
{
}



void CHarp::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CHarp::SetAni()
{
	switch (m_eCurState)
	{
	case CHarp::STATE_MOVE:
	
		break;
	case CHarp::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Harp"), 0.2f, CAnimator::STATE_LOOF);
		break;
		
	case CHarp::STATE_HIT:
		break;

	case CHarp::STATE_CHASE:
		break;
	}
}

void CHarp::Damaged(CGameObject * pOther)
{

}

CHarp * CHarp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHarp*		pInstance = new CHarp(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHarp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CHarp::Clone(void* pArg)
{
	CHarp*		pInstance = new CHarp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHarp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CHarp::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Player")
	{
		SetState(STATE_MOVE, DIR_L);
	}
}




void CHarp::Free()
{
	__super::Free();

}


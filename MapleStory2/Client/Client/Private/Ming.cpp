#include "stdafx.h"
#include "..\Public\Ming.h"
#include "GameInstance.h"


CMing::CMing(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CMing::CMing(const CMing & rhs)
	: CCreature(rhs)
{
}

HRESULT CMing::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CMing::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.5f, 4.0f, 5.f));
	m_pTransformCom->Set_Scaled(1.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CMing::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ming_StandL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ming_StandR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ming_MoveL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Ming_MoveR"), nullptr);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CMing::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{
	case Client::CMing::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CMing::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CMing::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CMing::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CMing::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CMing::Render()
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

void CMing::Tick_Idle(_float fTimeDelta)
{
	//멤버 변수로 0 초기화 해두고
	// fTimeDelta를 더한다
	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;
		if (m_fTime >= 3.f)
		{
			m_fTime = 0.f;
			SetState(STATE_MOVE, DIR_L);
		}
	}

	if (m_eDir == DIR_R)
	{
		m_fTime += fTimeDelta;
		if (m_fTime >= 3.f)
		{
			m_fTime = 0.f;
			SetState(STATE_MOVE, DIR_R);
		}
	}
}
void CMing::Tick_Move(_float fTimeDelta)
{
	if (m_eDir == DIR_R)
	{	
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_Right(fTimeDelta);

		if (m_fTime >= 3.f)
		{
			SetState(STATE_IDLE, DIR_L);
			m_fTime = 0.f;
		}
	}

	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_Left(fTimeDelta);

		if (m_fTime >= 3.f)
		{
			SetState(STATE_IDLE, DIR_R);
			m_fTime = 0.f;
		}
	}
}
void CMing::Tick_Hit(_float fTimeDelta)
{

}

void CMing::Tick_Chase(_float fTimeDelta)
{
}



void CMing::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CMing::SetAni()
{
	switch (m_eCurState)
	{
	case CMing::STATE_IDLE:
		if (m_eDir == DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Ming_StandL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Ming_StandR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CMing::STATE_MOVE:
		if(m_eDir==DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Ming_MoveL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Ming_MoveR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CMing::STATE_HIT:
		break;

	case CMing::STATE_CHASE:
		break;
	}
}

void CMing::Damaged(CGameObject * pOther)
{
}

CMing * CMing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMing*		pInstance = new CMing(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMing"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CMing::Clone(void* pArg)
{
	CMing*		pInstance = new CMing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMing"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CMing::Collision(CGameObject * pOther)
{

}




void CMing::Free()
{
	__super::Free();

}


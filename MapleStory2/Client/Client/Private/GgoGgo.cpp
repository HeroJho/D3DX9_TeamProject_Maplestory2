#include "stdafx.h"
#include "..\Public\GgoGgo.h"
#include "GameInstance.h"


CGgoGgo::CGgoGgo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CGgoGgo::CGgoGgo(const CGgoGgo & rhs)
	: CCreature(rhs)
{
}

HRESULT CGgoGgo::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CGgoGgo::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(11.f, 5.7f, -3.0f));
	m_pTransformCom->Set_Scaled(0.5f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CGgoGgo::SetUp_Components()
{
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_GgoGgo_IdleL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_GgoGgo_StandL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_GgoGgo_StandR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_GgoGgo_MoveL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_GgoGgo_MoveR"), nullptr);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CGgoGgo::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{
	case Client::CGgoGgo::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CGgoGgo::STATE_END:
		Tick_Stand(fTimeDelta);
		break;

	case Client::CGgoGgo::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CGgoGgo::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CGgoGgo::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CGgoGgo::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CGgoGgo::Render()
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
void CGgoGgo::Tick_Idle(_float fTimeDelta)
{

}
void CGgoGgo::Tick_Stand(_float fTimeDelta)
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

void CGgoGgo::Tick_Move(_float fTimeDelta)
{
	if (m_eDir == DIR_R)
	{	
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_Right(fTimeDelta);

		if (m_fTime >= 3.f)
		{
			SetState(STATE_END, DIR_L);
			m_fTime = 0.f;
		}
	}

	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_Left(fTimeDelta);

		if (m_fTime >= 3.f)
		{
			SetState(STATE_END, DIR_R);
			m_fTime = 0.f;
		}
	}
}
void CGgoGgo::Tick_Hit(_float fTimeDelta)
{

}

void CGgoGgo::Tick_Chase(_float fTimeDelta)
{
}



void CGgoGgo::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CGgoGgo::SetAni()
{
	switch (m_eCurState)
	{
	case CGgoGgo::STATE_END:
		if (m_eDir == DIR_L)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GgoGgo_StandL"), 0.3f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GgoGgo_StandR"), 0.3f, CAnimator::STATE_LOOF);
		break;
		
	case CGgoGgo::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GgoGgo_IdleL"), 0.3f, CAnimator::STATE_LOOF);
		break;
	case CGgoGgo::STATE_MOVE:
		if(m_eDir==DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GgoGgo_MoveL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GgoGgo_MoveR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CGgoGgo::STATE_HIT:
		break;

	case CGgoGgo::STATE_CHASE:
		break;
	}
}

void CGgoGgo::Damaged(CGameObject * pOther)
{

}

CGgoGgo * CGgoGgo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGgoGgo*		pInstance = new CGgoGgo(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGgoGgo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CGgoGgo::Clone(void* pArg)
{
	CGgoGgo*		pInstance = new CGgoGgo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGgoGgo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CGgoGgo::Collision(CGameObject * pOther)
{

}




void CGgoGgo::Free()
{
	__super::Free();

}


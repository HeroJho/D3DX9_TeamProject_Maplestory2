#include "stdafx.h"
#include "..\Public\Chick.h"
#include "GameInstance.h"


CChick::CChick(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CChick::CChick(const CChick & rhs)
	: CCreature(rhs)
{
}

HRESULT CChick::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CChick::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	
	CCreature::CRETUREDESC* pNpcDesc = (CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pNpcDesc->vPos);
	m_pTransformCom->Set_Scaled(0.3f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CChick::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chick_StandL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chick_StandR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chick_MoveL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chick_MoveR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Chick_IdleL"), nullptr);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CChick::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{
	case Client::CChick::STATE_END:
		Tick_Stand(fTimeDelta);
		break;
	case Client::CChick::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CChick::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CChick::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CChick::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CChick::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CChick::Render()
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
void CChick::Tick_Idle(_float fTimeDelta)
{

}
void CChick::Tick_Stand(_float fTimeDelta)
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
void CChick::Tick_Move(_float fTimeDelta)
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
void CChick::Tick_Hit(_float fTimeDelta)
{

}

void CChick::Tick_Chase(_float fTimeDelta)
{
}



void CChick::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CChick::SetAni()
{
	switch (m_eCurState)
	{
	case CChick::STATE_END:
		if (m_eDir == DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Chick_StandL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Chick_StandR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CChick::STATE_MOVE:
		if(m_eDir==DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Chick_MoveL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Chick_MoveR"), 0.3f, CAnimator::STATE_LOOF);
		break;
	case CChick::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Chick_IdleL"), 0.3f, CAnimator::STATE_LOOF);
		break;
		
	case CChick::STATE_HIT:
		break;

	case CChick::STATE_CHASE:
		break;
	}
}

void CChick::Damaged(CGameObject * pOther)
{

}

CChick * CChick::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChick*		pInstance = new CChick(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CChick"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CChick::Clone(void* pArg)
{
	CChick*		pInstance = new CChick(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CChick"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CChick::Collision(CGameObject * pOther)
{

}




void CChick::Free()
{
	__super::Free();

}


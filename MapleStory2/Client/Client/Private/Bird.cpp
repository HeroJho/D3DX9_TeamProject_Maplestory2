#include "stdafx.h"
#include "..\Public\Bird.h"
#include "GameInstance.h"


CBird::CBird(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBird::CBird(const CBird & rhs)
	: CCreature(rhs)
{
}

HRESULT CBird::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBird::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 0.5f;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.7f, -7.8f));
	m_pTransformCom->Set_Scaled(0.5f);
	m_fTime = 0.f;
	SetState(STATE_MOVE, DIR_R);
	m_fSpeed = 6.f;
	return S_OK;
}

HRESULT CBird::SetUp_Components()
{

	m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bird_StandL"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bird_StandR"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bird_MoveL"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bird_MoveR"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bird_ChaseR"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bird_JumpR"), nullptr);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.8f;

	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CBird::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{
	case Client::CBird::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CBird::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CBird::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CBird::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CBird::LateTick(_float fTimeDelta)
{
	Set_Billboard();
	
	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CBird::Render()
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

void CBird::Tick_Idle(_float fTimeDelta)
{
	//멤버 변수로 0 초기화 해두고
	// fTimeDelta를 더한다
	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;
		if (m_fTime >= 2.f)
		{
			m_fTime = 0.f;
			SetState(STATE_MOVE, DIR_L);
		}
	}

	if (m_eDir == DIR_R)
	{
		m_fTime += fTimeDelta;
		if (m_fTime >= 2.f)
		{
			m_fTime = 0.f;
			SetState(STATE_MOVE, DIR_R);
		}
	}
}
void CBird::Tick_Move(_float fTimeDelta)
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
void CBird::Tick_Hit(_float fTimeDelta)
{

}

void CBird::Tick_Chase(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime > 5.f)
	{
		Set_Dead();
		m_fTime = 0.f;
	}
	CTransform::TRANSFORMDESC ChaseDesc;
	ChaseDesc.fSpeedPerSec = m_fSpeed;
	ChaseDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_pTransformCom->Set_TransformDesc(ChaseDesc);

	_float3 vTargetPos = _float3(8.f, 10.f, 4.f);
	_float3	vLook = vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK,vLook);

	
	if (m_fSpeed >=2.f)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_fSpeed -= 0.1f;	

	}

	if (m_fSpeed < 2.f)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	//	m_pTransformCom->Go_RU(fTimeDelta);
	}
}




void CBird::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CBird::SetAni()
{
	switch (m_eCurState)
	{
	case CBird::STATE_IDLE:
		if (m_eDir == DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Bird_StandL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Bird_StandR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CBird::STATE_MOVE:
		if(m_eDir==DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Bird_MoveL"), 0.1f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Bird_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		break;

	case CBird::STATE_HIT:
		break;

	case CBird::STATE_CHASE:
		//if (m_eDir == DIR_L)
		//	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Bird_MoveL"), 0.3f, CAnimator::STATE_LOOF);
		//else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Bird_ChaseR"), 0.3f, CAnimator::STATE_LOOF);
			CGameInstance::Get_Instance()->PlaySound(L"Bird.mp3", 9, 1.f);
	//	break;
	}
}

void CBird::Damaged(CGameObject * pOther)
{

}

CBird * CBird::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBird*		pInstance = new CBird(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBird"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBird::Clone(void* pArg)
{
	CBird*		pInstance = new CBird(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBird"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBird::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Player")
		SetState(STATE_CHASE, DIR_R);
}




void CBird::Free()
{
	__super::Free();

}


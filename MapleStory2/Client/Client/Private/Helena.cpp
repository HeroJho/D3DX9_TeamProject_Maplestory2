#include "stdafx.h"
#include "..\Public\Helena.h"
#include "GameInstance.h"


CHelena::CHelena(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CHelena::CHelena(const CHelena & rhs)
	: CCreature(rhs)
{
}

HRESULT CHelena::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CHelena::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-9.f, 1.0f, -1.f));
	m_pTransformCom->Set_Scaled(1.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CHelena::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Helena_StandL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Helena_StandR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Helena_MoveL"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Helena_MoveR"), nullptr);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CHelena::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{
	case Client::CHelena::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CHelena::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CHelena::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CHelena::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CHelena::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CHelena::Render()
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

void CHelena::Tick_Idle(_float fTimeDelta)
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
void CHelena::Tick_Move(_float fTimeDelta)
{
	if (m_eDir == DIR_R)
	{	
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_RU(fTimeDelta);

		if (m_fTime >= 5.f)
		{
			SetState(STATE_IDLE, DIR_L);
			m_fTime = 0.f;
		}
	}

	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_LD(fTimeDelta);

		if (m_fTime >= 5.f)
		{
			SetState(STATE_IDLE, DIR_R);
			m_fTime = 0.f;
		}
	}
}
void CHelena::Tick_Hit(_float fTimeDelta)
{

}

void CHelena::Tick_Chase(_float fTimeDelta)
{
}



void CHelena::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CHelena::SetAni()
{
	switch (m_eCurState)
	{
	case CHelena::STATE_IDLE:
		if (m_eDir == DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_StandL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_StandR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CHelena::STATE_MOVE:
		if(m_eDir==DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_MoveL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_MoveR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CHelena::STATE_HIT:
		break;

	case CHelena::STATE_CHASE:
		break;
	}
}

void CHelena::Damaged(CGameObject * pOther)
{

}

CHelena * CHelena::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHelena*		pInstance = new CHelena(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHelena"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CHelena::Clone(void* pArg)
{
	CHelena*		pInstance = new CHelena(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHelena"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CHelena::Collision(CGameObject * pOther)
{

}




void CHelena::Free()
{
	__super::Free();

}


#include "stdafx.h"
#include "..\Public\Grass.h"
#include "GameInstance.h"


CGrass::CGrass(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CGrass::CGrass(const CGrass & rhs)
	: CCreature(rhs)
{
}

HRESULT CGrass::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CGrass::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	
	CCreature::CRETUREDESC* pNpcDesc = (CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pNpcDesc->vPos);
	m_pTransformCom->Set_Scaled(0.9f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CGrass::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Grass"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GrassIdle"), nullptr);


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CGrass::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{

	case Client::CGrass::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CGrass::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CGrass::STATE_HIT:
		
		Tick_Hit(fTimeDelta);
		break;

	case Client::CGrass::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CGrass::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CGrass::Render()
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
void CGrass::Tick_Idle(_float fTimeDelta)
{

}
void CGrass::Tick_Stand(_float fTimeDelta)
{

}
void CGrass::Tick_Move(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime > 1.f)
	{
		SetState(STATE_IDLE, DIR_L);
		m_fTime = 0;
	}
}
void CGrass::Tick_Hit(_float fTimeDelta)
{

}

void CGrass::Tick_Chase(_float fTimeDelta)
{
}



void CGrass::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CGrass::SetAni()
{
	switch (m_eCurState)
	{
	case CGrass::STATE_MOVE:
		CGameInstance::Get_Instance()->PlaySound(L"Leaf.mp3", 8, 1.f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Grass"), 0.2f, CAnimator::STATE_LOOF);
		break;
	case CGrass::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GrassIdle"), 0.2f, CAnimator::STATE_LOOF);
		break;
		
	case CGrass::STATE_HIT:
		break;

	case CGrass::STATE_CHASE:
		break;
	}
}

void CGrass::Damaged(CGameObject * pOther)
{

}

CGrass * CGrass::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGrass*		pInstance = new CGrass(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGrass"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CGrass::Clone(void* pArg)
{
	CGrass*		pInstance = new CGrass(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGrass"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CGrass::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Player")
	{
		SetState(STATE_MOVE, DIR_L);
	}
}




void CGrass::Free()
{
	__super::Free();

}


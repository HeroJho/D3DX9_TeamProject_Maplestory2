#include "stdafx.h"
#include "..\Public\SlimeStone.h"
#include "GameInstance.h"


CSlimeStone::CSlimeStone(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CSlimeStone::CSlimeStone(const CSlimeStone & rhs)
	: CCreature(rhs)
{
}

HRESULT CSlimeStone::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CSlimeStone::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	
	CCreature::CRETUREDESC* pSlimeStoneDesc = (CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pSlimeStoneDesc->vPos);
	m_pTransformCom->Set_Scaled(1.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CSlimeStone::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SlimeStone"), nullptr);
		

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CSlimeStone::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{

	case Client::CSlimeStone::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CSlimeStone::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CSlimeStone::STATE_HIT:
		
		Tick_Hit(fTimeDelta);
		break;

	case Client::CSlimeStone::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CSlimeStone::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CSlimeStone::Render()
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
void CSlimeStone::Tick_Idle(_float fTimeDelta)
{

}
void CSlimeStone::Tick_Stand(_float fTimeDelta)
{

}
void CSlimeStone::Tick_Move(_float fTimeDelta)
{

}
void CSlimeStone::Tick_Hit(_float fTimeDelta)
{

}

void CSlimeStone::Tick_Chase(_float fTimeDelta)
{
}



void CSlimeStone::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CSlimeStone::SetAni()
{
	switch (m_eCurState)
	{
	case CSlimeStone::STATE_MOVE:
		break;
	case CSlimeStone::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_SlimeStone"), 0.2f, CAnimator::STATE_LOOF);

		break;
		
	case CSlimeStone::STATE_HIT:
		break;

	case CSlimeStone::STATE_CHASE:
		break;
	}
}

void CSlimeStone::Damaged(CGameObject * pOther)
{

}

CSlimeStone * CSlimeStone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlimeStone*		pInstance = new CSlimeStone(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSlimeStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CSlimeStone::Clone(void* pArg)
{
	CSlimeStone*		pInstance = new CSlimeStone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSlimeStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CSlimeStone::Collision(CGameObject * pOther)
{
	
}




void CSlimeStone::Free()
{
	__super::Free();

}


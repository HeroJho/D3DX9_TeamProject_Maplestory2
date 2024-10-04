#include "stdafx.h"
#include "..\Public\GreenGate.h"

#include "GameInstance.h"


CGreenGate::CGreenGate(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CGreenGate::CGreenGate(const CGreenGate & rhs)
	: CCreature(rhs)
{
}




HRESULT CGreenGate::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CGreenGate::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(6.8f, 1.7f, 7.3f));
	m_pTransformCom->Set_Scaled(4.5f);

	SetState(STATE_IDLE, DIR_END);



	return S_OK;
}




HRESULT CGreenGate::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_GreenGate"), nullptr);

	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CGreenGate::Tick(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case Client::CGreenGate::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CGreenGate::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CGreenGate::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CGreenGate::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

}
void CGreenGate::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}
HRESULT CGreenGate::Render()
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





void CGreenGate::Tick_Idle(_float fTimeDelta)
{
}
void CGreenGate::Tick_Move(_float fTimeDelta)
{
}
void CGreenGate::Tick_Hit(_float fTimeDelta)
{
}

void CGreenGate::Tick_Chase(_float fTimeDelta)
{
}



void CGreenGate::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CGreenGate::SetAni()
{
	switch (m_eCurState)
	{
	case CGreenGate::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenGate"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CGreenGate::STATE_MOVE:
	{
	}
	break;
	case CGreenGate::STATE_HIT:
		break;
	case CGreenGate::STATE_CHASE:
		break;
	}
}

void CGreenGate::Damaged(CGameObject * pOther)
{
}






CGreenGate * CGreenGate::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGreenGate*		pInstance = new CGreenGate(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGreenGate"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CGreenGate::Clone(void* pArg)
{
	CGreenGate*		pInstance = new CGreenGate(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGreenGate"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CGreenGate::Collision(CGameObject * pOther)
{
}




void CGreenGate::Free()
{
	__super::Free();

}


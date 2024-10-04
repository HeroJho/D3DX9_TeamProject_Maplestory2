#include "stdafx.h"
#include "..\Public\PurpleGate.h"

#include "GameInstance.h"


CPurpleGate::CPurpleGate(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CPurpleGate::CPurpleGate(const CPurpleGate & rhs)
	: CCreature(rhs)
{
}




HRESULT CPurpleGate::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CPurpleGate::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-10.5f, 1.5f, 0.f));
	m_pTransformCom->Set_Scaled(4.5f);

	SetState(STATE_IDLE, DIR_END);



	return S_OK;
}




HRESULT CPurpleGate::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_PurpleGate"), nullptr);

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




void CPurpleGate::Tick(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case Client::CPurpleGate::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CPurpleGate::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CPurpleGate::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CPurpleGate::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

}
void CPurpleGate::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}
HRESULT CPurpleGate::Render()
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





void CPurpleGate::Tick_Idle(_float fTimeDelta)
{
}
void CPurpleGate::Tick_Move(_float fTimeDelta)
{
}
void CPurpleGate::Tick_Hit(_float fTimeDelta)
{
}

void CPurpleGate::Tick_Chase(_float fTimeDelta)
{
}



void CPurpleGate::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CPurpleGate::SetAni()
{
	switch (m_eCurState)
	{
	case CPurpleGate::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_PurpleGate"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CPurpleGate::STATE_MOVE:
	{
	}
	break;
	case CPurpleGate::STATE_HIT:
		break;
	case CPurpleGate::STATE_CHASE:
		break;
	}
}

void CPurpleGate::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	
	SetState(STATE_HIT,DIR_END);



	Safe_Release(pGameInstance);
}






CPurpleGate * CPurpleGate::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPurpleGate*		pInstance = new CPurpleGate(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPurpleGate"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CPurpleGate::Clone(void* pArg)
{
	CPurpleGate*		pInstance = new CPurpleGate(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPurpleGate"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CPurpleGate::Collision(CGameObject * pOther)
{
}




void CPurpleGate::Free()
{
	__super::Free();

}


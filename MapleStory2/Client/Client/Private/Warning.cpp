#include "stdafx.h"
#include "..\Public\Warning.h"

#include "GameInstance.h"


CWarning::CWarning(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CWarning::CWarning(const CWarning & rhs)
	: CCreature(rhs)
{
}




HRESULT CWarning::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CWarning::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(30.f, 3.f, -8.f));
	m_pTransformCom->Set_Scaled(1.f);

	SetState(STATE_IDLE, DIR_END);



	return S_OK;
}




HRESULT CWarning::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Warning"), nullptr);

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




void CWarning::Tick(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case Client::CWarning::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CWarning::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CWarning::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CWarning::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

}
void CWarning::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}
HRESULT CWarning::Render()
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





void CWarning::Tick_Idle(_float fTimeDelta)
{
}
void CWarning::Tick_Move(_float fTimeDelta)
{
}
void CWarning::Tick_Hit(_float fTimeDelta)
{
}

void CWarning::Tick_Chase(_float fTimeDelta)
{
}



void CWarning::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CWarning::SetAni()
{
	switch (m_eCurState)
	{
	case CWarning::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Warning"), 0.3f, CAnimator::STATE_ONCE);
		break;
	case CWarning::STATE_MOVE:
	{
	}
	break;
	case CWarning::STATE_HIT:
		break;
	case CWarning::STATE_CHASE:
		break;
	}
}

void CWarning::Damaged(CGameObject * pOther)
{
}






CWarning * CWarning::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWarning*		pInstance = new CWarning(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWarning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CWarning::Clone(void* pArg)
{
	CWarning*		pInstance = new CWarning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CWarning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CWarning::Collision(CGameObject * pOther)
{
}




void CWarning::Free()
{
	__super::Free();

}


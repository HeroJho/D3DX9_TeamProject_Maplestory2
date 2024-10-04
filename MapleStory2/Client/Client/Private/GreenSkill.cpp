#include "stdafx.h"
#include "..\Public\GreenSkill.h"

#include "GameInstance.h"


CGreenSkill::CGreenSkill(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CGreenSkill::CGreenSkill(const CGreenSkill & rhs)
	: CCreature(rhs)
{
}




HRESULT CGreenSkill::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CGreenSkill::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	

	m_fDead = 0;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 4.f, 0.f));
	m_pTransformCom->Set_Scaled(8.f);

	SetState(STATE_IDLE, DIR_END);

	CGameInstance::Get_Instance()->PlaySound(L"GreenSlimeSkill.wav", 13, 0.6f);

	return S_OK;
}




HRESULT CGreenSkill::SetUp_Components()
{

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenSkill"), nullptr);
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




void CGreenSkill::Tick(_float fTimeDelta)
{

	

	switch (m_eCurState)
	{
	case Client::CGreenSkill::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CGreenSkill::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CGreenSkill::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CGreenSkill::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

}
void CGreenSkill::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	m_fDead += fTimeDelta;

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_NPC, this);


	if (m_fDead > 2.3f)
		Set_Dead();
}
HRESULT CGreenSkill::Render()
{
	_float4x4		Matrix;
	D3DXMatrixIdentity(&Matrix);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


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





void CGreenSkill::Tick_Idle(_float fTimeDelta)
{
}
void CGreenSkill::Tick_Move(_float fTimeDelta)
{
}
void CGreenSkill::Tick_Hit(_float fTimeDelta)
{
}

void CGreenSkill::Tick_Chase(_float fTimeDelta)
{
}



void CGreenSkill::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CGreenSkill::SetAni()
{
	switch (m_eCurState)
	{
	case CGreenSkill::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSkill"), 0.2f, CAnimator::STATE_LOOF);
		break;
	case CGreenSkill::STATE_MOVE:
	{
	}
	break;
	case CGreenSkill::STATE_HIT:
		break;
	case CGreenSkill::STATE_CHASE:
		break;
	}
}

void CGreenSkill::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


	SetState(STATE_HIT, DIR_END);


	Safe_Release(pGameInstance);
}






CGreenSkill * CGreenSkill::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGreenSkill*		pInstance = new CGreenSkill(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGreenSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CGreenSkill::Clone(void* pArg)
{
	CGreenSkill*		pInstance = new CGreenSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGreenSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CGreenSkill::Collision(CGameObject * pOther)
{
}




void CGreenSkill::Free()
{
	__super::Free();

}


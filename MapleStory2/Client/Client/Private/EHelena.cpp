#include "stdafx.h"
#include "..\Public\EHelena.h"
#include "GameInstance.h"
#include "CutSceneManager.h"
#include "UIManager.h"

CEHelena::CEHelena(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CEHelena::CEHelena(const CEHelena & rhs)
	: CCreature(rhs)
{
}

HRESULT CEHelena::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CEHelena::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3( 16.f, 3.0f, 2.f));
	m_pTransformCom->Set_Scaled(1.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CEHelena::SetUp_Components()
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

void CEHelena::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{
	case Client::CEHelena::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CEHelena::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CEHelena::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CEHelena::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CEHelena::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CEHelena::Render()
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

void CEHelena::Tick_Idle(_float fTimeDelta)
{
}
void CEHelena::Tick_Move(_float fTimeDelta)
{
}
void CEHelena::Tick_Hit(_float fTimeDelta)
{

}

void CEHelena::Tick_Chase(_float fTimeDelta)
{
}



void CEHelena::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CEHelena::SetAni()
{
	switch (m_eCurState)
	{
	case CEHelena::STATE_IDLE:
		if (m_eDir == DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_StandL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_StandR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CEHelena::STATE_MOVE:
		if(m_eDir==DIR_L)
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_MoveL"), 0.3f, CAnimator::STATE_LOOF);
		else
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Helena_MoveR"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CEHelena::STATE_HIT:
		break;

	case CEHelena::STATE_CHASE:
		break;
	}
}

void CEHelena::Damaged(CGameObject * pOther)
{

}

CEHelena * CEHelena::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEHelena*		pInstance = new CEHelena(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CEHelena"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CEHelena::Clone(void* pArg)
{
	CEHelena*		pInstance = new CEHelena(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CEHelena"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CEHelena::Collision(CGameObject * pOther)
{
	if ("Tag_Player" == pOther->Get_Tag())
	{
		if (CGameInstance::Get_Instance()->Key_Down(DIK_SPACE))
		{
			CCutSceneManager::Get_Instance()->Get_MainCam()->SetFocus(this, _float3{ 0.f, 1.f, -4.f }, 5.f);
			CUIManager::Get_Instance()->On_EHelenaChat();
		}
	}
}




void CEHelena::Free()
{
	__super::Free();

}


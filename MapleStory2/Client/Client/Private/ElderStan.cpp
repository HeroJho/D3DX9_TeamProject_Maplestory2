#include "stdafx.h"
#include "..\Public\ElderStan.h"

#include "GameInstance.h"

#include "CutSceneManager.h"
#include "QuestManager.h"
#include "ToolManager.h"


CElderStan::CElderStan(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CElderStan::CElderStan(const CElderStan & rhs)
	: CCreature(rhs)
{
}




HRESULT CElderStan::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CElderStan::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(9.f, 6.0f, -1.0f));
	m_pTransformCom->Set_Scaled(1.5f);

	SetState(STATE_IDLE, DIR_END);
	CCutSceneManager::Get_Instance()->Set_Jang(this);


	return S_OK;
}




HRESULT CElderStan::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ElderStan_Idle"), nullptr);

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




void CElderStan::Tick(_float fTimeDelta)
{
	if(CToolManager::Get_Instance()->Get_CurLevel() == LEVEL_DHENESYS)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(14.1, 7.0f, -1.f));

	if (CToolManager::Get_Instance()->Get_CurLevel() == LEVEL_WHITEMAGE)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.5f, 1.f, -1.f));

	switch (m_eCurState)
	{
	case Client::CElderStan::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CElderStan::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CElderStan::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CElderStan::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

	if (GetKeyState('B'))
		m_bEnding = true;

}
void CElderStan::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if(m_bEnding)
	m_pTransformCom->Go_Gravity(fTimeDelta);

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_NPC, this);
}
HRESULT CElderStan::Render()
{
	if (!(CCutSceneManager::Get_Instance()->Get_jangRander()))
		return S_OK;

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





void CElderStan::Tick_Idle(_float fTimeDelta)
{
}
void CElderStan::Tick_Move(_float fTimeDelta)
{
}
void CElderStan::Tick_Hit(_float fTimeDelta)
{
}

void CElderStan::Tick_Chase(_float fTimeDelta)
{
}



void CElderStan::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CElderStan::SetAni()
{
	switch (m_eCurState)
	{
	case CElderStan::STATE_IDLE:
		if (CQuestManager::Get_Instance()->Get_QuestNum() == 6)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_ElderStan_Idle"), 0.3f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_ElderStan_Idle"), 0.3f, CAnimator::STATE_ONCE);
		break;
	case CElderStan::STATE_MOVE:
	{
	}
	break;
	case CElderStan::STATE_HIT:
		break;
	case CElderStan::STATE_CHASE:
		break;
	}
}

void CElderStan::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	
	SetState(STATE_HIT,DIR_END);



	Safe_Release(pGameInstance);
}






CElderStan * CElderStan::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CElderStan*		pInstance = new CElderStan(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CElderStan"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CElderStan::Clone(void* pArg)
{
	CElderStan*		pInstance = new CElderStan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CElderStan"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CElderStan::Collision(CGameObject * pOther)
{
}




void CElderStan::Free()
{
	__super::Free();

}


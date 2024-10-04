#include "stdafx.h"
#include "..\Public\KingSlime.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "UIManager.h"
#include "CutSceneManager.h"
#include "ParticleManager.h"
#include "ToolManager.h"

CKingSlime::CKingSlime(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CKingSlime::CKingSlime(const CKingSlime & rhs)
	: CCreature(rhs)
{
}

HRESULT CKingSlime::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CKingSlime::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Monster";
	m_iHp = 3;
	m_iIndexNum = -1;
	m_iDamage = 3;
	m_fCountDead = 0;
	m_bAttack = false;


	vPos.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;

	m_fColRad = 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.f, 16.f, 4.f));
	m_pTransformCom->Set_Scaled(4.f);

	//SetState(STATE_CUTSCEEN, DIR_END);

	SetState(STATE_IDLE, DIR_END);

	SetShadow(LEVEL_ELENYA, 4.5f);

	return S_OK;
}




HRESULT CKingSlime::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.5f, 3.f, 0.5f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KingSlime_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KingSlime_MoveR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KingSlime_Hit"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KingSlime_HitR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KingSlime_Die"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KingSlime_DieR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_KingSlime_Idle"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.3f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CKingSlime::Die()
{
	if (m_eDir == DIR_L)
	{
		SetState(STATE_DIE, DIR_L);
	}
	else if (m_eDir == DIR_R)
	{
		SetState(STATE_DIE, DIR_R);
	}
	//m_fColRad = 0;
}





void CKingSlime::Tick(_float fTimeDelta)
{

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < 10)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(20.f, 16.f, 4.f));
	}

	switch (m_eCurState)
	{
	case Client::CKingSlime::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CKingSlime::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CKingSlime::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CKingSlime::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	case Client::CKingSlime::STATE_ATTACK:
		Tick_Attack(fTimeDelta);
		break;
	case Client::CKingSlime::STATE_DIE:
		Tick_Die(fTimeDelta);
		break;
	case Client::CKingSlime::STATE_CUTSCEEN:
		Tick_CutScene(fTimeDelta);
		break;
	case Client::CKingSlime::STATE_END:
		Tick_End(fTimeDelta);
		break;
	}

}

void CKingSlime::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	_float3 Mushpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (Mushpos.y < 3.5f)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(Mushpos.x, 3.5f, Mushpos.z));

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		if (m_eCurState != STATE_DIE && m_eCurState != STATE_ATTACK)
			SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	if (m_iHp > 0)
	{
		m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	}

	Set_Billboard();
}
HRESULT CKingSlime::Render()
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





void CKingSlime::Tick_Idle(_float fTimeDelta)
{
}
void CKingSlime::Tick_Move(_float fTimeDelta)
{
}
void CKingSlime::Tick_Hit(_float fTimeDelta)
{
}

void CKingSlime::Tick_Chase(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < vPlayerPos.x)
		SetState(STATE_CHASE, DIR_R);
	else
		SetState(STATE_CHASE, DIR_L);


	m_pTransformCom->Chase(vPlayerPos + _float3(0.1f,0.f,0.1f), fTimeDelta);

	Safe_Release(pGameInstance);
}

void CKingSlime::Tick_Attack(_float fTimeDelta)
{
}

void CKingSlime::Cut_Attack(_float fTimeDelta)
{
}


void CKingSlime::Tick_Die(_float fTimeDelta)
{
	m_fCountDead += fTimeDelta;
	if (m_fCountDead >= 2.1f)
		Set_Dead();
}

void CKingSlime::Tick_CutScene(_float fTimeDelta)
{
}

void CKingSlime::Tick_End(_float fTimeDelta)
{
}



void CKingSlime::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CKingSlime::SetAni()
{
	switch (m_eCurState)
	{
	case CKingSlime::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_Idle"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CKingSlime::STATE_MOVE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CKingSlime::STATE_HIT:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_HitR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_Hit"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case CKingSlime::STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CKingSlime::STATE_DIE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_DieR"), 0.3f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_KingSlime_Die"), 0.3f, CAnimator::STATE_ONCE);
		break;
	}
}

void CKingSlime::MakeItem()
{
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CInvenManager::Get_Instance()->MakeItem(CInvenManager::TYPE_STUFF, 2, L"SlimeInfo", vPos, LEVEL_ELENYA);
}

void CKingSlime::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_eCurState != STATE_ATTACK)
	{
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < vPlayerPos.x)
			SetState(STATE_HIT, DIR_R);
		else
			SetState(STATE_HIT, DIR_L);
	}


	Safe_Release(pGameInstance);
	if (pOther->Get_Tag() == "Player_Skill")
	m_iHp -= pOther->Get_Damage();
	if (m_iHp <= 0)
	{
		CGameInstance::Get_Instance()->PlaySound(L"SlimeDie.wav", 13, 1.f);
		MakeItem();
		Die();
		CUIManager::Get_Instance()->Set_PlayerExp(40);
	}
}






CKingSlime* CKingSlime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKingSlime*		pInstance = new CKingSlime(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CKingSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject* CKingSlime::Clone(void* pArg)
{
	CKingSlime*		pInstance = new CKingSlime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CKingSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CKingSlime::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Cube")
	{
		if (m_bLanding && m_bShake)
		{
			CCutSceneManager::Get_Instance()->Get_MainCam()->Start_AttackShaking();
			CParticleManager::Get_Instance()->BlueMushRoom_Lend(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			m_bShake = false;


			CCreature* pPlayer = (CCreature*)CToolManager::Get_Instance()->GetPlayer();
			CTransform* pPlayerTran = (CTransform*)pPlayer->Get_ComponentPtr(TEXT("Com_Transform"));
			_float3 PlayerPos = pPlayerTran->Get_State(CTransform::STATE_POSITION);
			_float3 MePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			_float PlayerY = pPlayerTran->Get_Vel();
			if(-0.1f < PlayerY && 0.1f > PlayerY)
			{
				_float3 vDir = PlayerPos - MePos;
				vDir.y = 0.f;
				pPlayer->SetKnockBack(7.f, 8.f, vDir);
			}

		}
	}
	

}




void CKingSlime::Free()
{
	__super::Free();

}


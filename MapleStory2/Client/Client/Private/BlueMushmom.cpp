#include "stdafx.h"
#include "..\Public\BlueMushmom.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "UIManager.h"
#include "BlueMushmom.h"
#include "CutSceneManager.h"
#include "ParticleManager.h"
#include "ToolManager.h"

CBlueMushmom::CBlueMushmom(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBlueMushmom::CBlueMushmom(const CBlueMushmom & rhs)
	: CCreature(rhs)
{
}

HRESULT CBlueMushmom::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBlueMushmom::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Monster";
	m_iHp = 20;
	m_iIndexNum = -1;
	m_fCountDead = 0;
	m_fCountJump = 5.f;
	m_bAttack = false;
	m_fCountLanding = 0.f;
	m_bLanding = false;
	m_fCountLand = 0.f;
	m_iDamage = 4;
	CUIManager::Get_Instance()->Get_BlueMushmomHp(m_iHp);

	vPos.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;

	m_fColRad = 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(45.f, 10.f, -9.f));
	m_pTransformCom->Set_Scaled(4.f);

	SetState(STATE_CUTSCEEN, DIR_END);

	SetShadow(LEVEL_HENESYS, 4.5f);

	return S_OK;
}




HRESULT CBlueMushmom::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.5f, 2.f, 0.5f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_MoveR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_Hit"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_HitR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_Die"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_DieR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_Attack"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_AttackR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueMushmom_Idle"), nullptr);
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

void CBlueMushmom::Die()
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





void CBlueMushmom::Tick(_float fTimeDelta)
{
	if (STATE_CUTSCEEN != m_eCurState)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

		_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

		m_fCountJump += fTimeDelta;

		Safe_Release(pGameInstance);

		if (vPlayerPos.x < 30.f)
		{
			SetState(STATE_END, DIR_END);
			m_iHp = 20;
		}

		if (fabs(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x - vPlayerPos.x) < 2.f)
		{
			if (m_fCountJump >= 10.f && m_eCurState != STATE_ATTACK)
			{
				if (m_eCurState != STATE_HIT && m_eCurState != STATE_DIE)
					if (m_eDir == DIR_R)
						SetState(STATE_ATTACK, DIR_R);
					else
						SetState(STATE_ATTACK, DIR_L);
			}
		}

		else if (fabs(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x - vPlayerPos.x) < 2.f)
		{
			if (m_eCurState != STATE_HIT && m_eCurState != STATE_DIE &&  m_eCurState != STATE_ATTACK)
				if (m_eDir == DIR_R)
					SetState(STATE_CHASE, DIR_R);
				else
					SetState(STATE_CHASE, DIR_L);
		}




		CUIManager::Get_Instance()->Get_BlueMushmomHp(m_iHp);
	}



	switch (m_eCurState)
	{
	case Client::CBlueMushmom::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CBlueMushmom::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CBlueMushmom::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CBlueMushmom::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	case Client::CBlueMushmom::STATE_ATTACK:
		Tick_Attack(fTimeDelta);
		break;
	case Client::CBlueMushmom::STATE_DIE:
		Tick_Die(fTimeDelta);
		break;
	case Client::CBlueMushmom::STATE_CUTSCEEN:
		Tick_CutScene(fTimeDelta);
		break;
	case Client::CBlueMushmom::STATE_END:
		Tick_End(fTimeDelta);
		break;
	}

}

void CBlueMushmom::LateTick(_float fTimeDelta)
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
HRESULT CBlueMushmom::Render()
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





void CBlueMushmom::Tick_Idle(_float fTimeDelta)
{
}
void CBlueMushmom::Tick_Move(_float fTimeDelta)
{
}
void CBlueMushmom::Tick_Hit(_float fTimeDelta)
{
}

void CBlueMushmom::Tick_Chase(_float fTimeDelta)
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

void CBlueMushmom::Tick_Attack(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


	// 내려오고 나서 상태를 바꾸는 시간
	m_fCountLand += fTimeDelta;

	//어느정도 시간이 지나고 점프를 시키는 시간
	if (!m_bLanding)
		m_fCountLanding += fTimeDelta;

	if (m_fCountLanding > 0.5f)
	{
		m_pTransformCom->Set_Vel(30.0f);
		m_fCountJump = 0;
		m_fCountLanding = 0;
		m_bLanding = true;
		m_bShake = true;
	}

	// 점프를 시작하고 5초 있다가 상태를 쫒아가게 바꿈
	if (m_fCountLand > 3.5f)
	{
		if (m_eDir == DIR_R)
			SetState(STATE_CHASE, DIR_R);
		else
			SetState(STATE_CHASE, DIR_L);
		m_bLanding = false;
		m_fCountLand = 0;
	}

	if (5.5f < m_pTransformCom->Get_State(CTransform::STATE_POSITION).y)
		m_pTransformCom->Chase(vPlayerPos, fTimeDelta * 5);

	Safe_Release(pGameInstance);
}

void CBlueMushmom::Cut_Attack(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	// 내려오고 나서 상태를 바꾸는 시간
	m_fCountLand += fTimeDelta;

	//어느정도 시간이 지나고 점프를 시키는 시간
	if (!m_bLanding)
		m_fCountLanding += fTimeDelta;

	if (m_fCountLanding > 0.5f)
	{
		m_pTransformCom->Set_Vel(60.0f);
		m_fCountJump = 0;
		m_fCountLanding = 0;
		m_bLanding = true;
		m_bShake = true;
	}

	// 점프를 시작하고 5초 있다가 상태를 쫒아가게 바꿈
	if (m_fCountLand > 5.0f)
	{
		if (m_eDir == DIR_R)
			SetState(STATE_CHASE, DIR_R);
		else
			SetState(STATE_CHASE, DIR_L);
		m_bLanding = false;
		m_fCountLand = 0;
		CGameInstance::Get_Instance()->PlaySound(L"donno.wav", 12, 2.f);
	}

	if (5.5f < m_pTransformCom->Get_State(CTransform::STATE_POSITION).y)
		m_pTransformCom->Chase(_float3(39.f, 3.f, -9.f), fTimeDelta * 10);



	Safe_Release(pGameInstance);
}


void CBlueMushmom::Tick_Die(_float fTimeDelta)
{
	m_fCountDead += fTimeDelta;
	if (m_fCountDead >= 2.0f)
		Set_Dead();
}

void CBlueMushmom::Tick_CutScene(_float fTimeDelta)
{
	m_fCutTimeAcc += fTimeDelta;
	if(4.f < m_fCutTimeAcc)
		Cut_Attack(fTimeDelta);
}

void CBlueMushmom::Tick_End(_float fTimeDelta)
{
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >= 38.9f)
		SetState(STATE_IDLE, DIR_END);
	else
		m_pTransformCom->Chase(_float3(39.f, 3.f, -9.f), fTimeDelta * 5);

}



void CBlueMushmom::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CBlueMushmom::SetAni()
{
	switch (m_eCurState)
	{
	case CBlueMushmom::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_Idle"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CBlueMushmom::STATE_MOVE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CBlueMushmom::STATE_HIT:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_HitR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_Hit"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case CBlueMushmom::STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CBlueMushmom::STATE_DIE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_DieR"), 0.3f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_Die"), 0.3f, CAnimator::STATE_ONCE);
		break;

	case CBlueMushmom::STATE_ATTACK:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_AttackR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_Attack"), 0.5f, CAnimator::STATE_ONCE);
		CGameInstance::Get_Instance()->PlaySound(L"MushJump.mp3", 10, 1.f);
		break;
	case CBlueMushmom::STATE_CUTSCEEN:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_Idle"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CBlueMushmom::STATE_END:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueMushmom_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		break;
	}
}

void CBlueMushmom::Damaged(CGameObject * pOther)
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

	CGameInstance::Get_Instance()->PlaySound(L"MushDamage.mp3", 10, 1.f);

	if (m_iHp <= 0)
	{
		CGameInstance::Get_Instance()->PlaySound(L"OrangeMushroomDie.wav", 1, 1.f);
		CQuestManager::Get_Instance()->Hunting(TEXT("BlueMushmom"));
		Die();
		CUIManager::Get_Instance()->Set_PlayerExp(80);
	}
}






CBlueMushmom * CBlueMushmom::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlueMushmom*		pInstance = new CBlueMushmom(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlueMushmom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBlueMushmom::Clone(void* pArg)
{
	CBlueMushmom*		pInstance = new CBlueMushmom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlueMushmom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBlueMushmom::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Cube")
	{
		if (m_bLanding && m_bShake)
		{
			CGameInstance::Get_Instance()->PlaySound(L"MushDown.mp3", 10, 1.f);

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




void CBlueMushmom::Free()
{
	__super::Free();

}


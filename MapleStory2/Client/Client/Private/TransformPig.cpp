#include "stdafx.h"
#include "..\Public\TransformPig.h"

#include "GameInstance.h"
#include "Spawner.h"
#include "SpawnerManager.h"
#include "QuestManager.h"
#include "ToolManager.h"

CTransformPig::CTransformPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CTransformPig::CTransformPig(const CTransformPig & rhs)
	: CCreature(rhs)
{
}




HRESULT CTransformPig::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CTransformPig::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Monster";

	m_iHp = 20;
	m_iDamage = 3;
	m_fCountDead = 0;

	m_fCountAttack = 0;


	CCreature::CRETUREDESC* pDesc = (CCreature::CRETUREDESC*)pArg;

	//CSpawner::SPAWNERINFO* pMonsterDesc = (CSpawner::SPAWNERINFO*)pArg;

	//m_iIndexNum = pMonsterDesc->SpawnerNum;

	//m_fColRad = pMonsterDesc->MonsterColRad;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
	m_pTransformCom->Set_Scaled(5.f);
	m_bDir = false;

	m_fStartPos = pDesc->vPos;

	// 랜덤으로 어느방향으로 움직일지와 거리를 생성한다
	m_iMove = CGameInstance::Get_Instance()->Get_Random(0, 4);
	m_fDistance = _float(CGameInstance::Get_Instance()->Get_Random(1, 5));


	switch (m_iMove)
	{
	case MOVE_R:
		SetState(STATE_MOVE, DIR_R);
		m_fEndPos.x = m_fStartPos.x + m_fDistance;
		break;
	case MOVE_L:
		SetState(STATE_MOVE, DIR_L);
		m_fEndPos.x = m_fStartPos.x - m_fDistance;
		break;
	case MOVE_U:
		SetState(STATE_MOVE, DIR_R);
		m_fEndPos.z = m_fStartPos.z + m_fDistance;
		break;
	case MOVE_D:
		SetState(STATE_MOVE, DIR_L);
		m_fEndPos.z = m_fStartPos.z - m_fDistance;
		break;
	case MOVE_END:
		SetState(STATE_IDLE, DIR_END);
		break;
	}

	// SetShadow(pMonsterDesc->Level, 1.5f);

	return S_OK;
}




HRESULT CTransformPig::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.5f, 2.f, 0.5f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_Idle"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_Hit"), nullptr);
											
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_MoveR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_HitR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_DieR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_Die"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_AttackR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TransformPig_Attack"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.7f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CTransformPig::Die()
{
	if (m_eDir == DIR_L)
	{
		SetState(STATE_DIE, DIR_L);
	}
	else if (m_eDir == DIR_R)
	{
		SetState(STATE_DIE, DIR_R);
	}
	m_fColRad = 0;
}



void CTransformPig::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (fabs(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x - vPlayerPos.x) < 2.f && fabs(m_pTransformCom->Get_State(CTransform::STATE_POSITION).z - vPlayerPos.z) < 2.f)
	{
		if (m_eCurState != STATE_HIT && m_eCurState != STATE_DIE)
		{
			if (m_eDir == DIR_R)
				SetState(STATE_ATTACK, DIR_R);
			else
				SetState(STATE_ATTACK, DIR_L);
		}
	}

	else if (fabs(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x - vPlayerPos.x) < 4.f)
	{
		if (m_eCurState != STATE_HIT && m_eCurState != STATE_DIE &&  m_eCurState != STATE_ATTACK)
			if (m_eDir == DIR_R)
				SetState(STATE_CHASE, DIR_R);
			else
				SetState(STATE_CHASE, DIR_L);
	}


	switch (m_eCurState)
	{
	case Client::CTransformPig::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CTransformPig::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CTransformPig::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CTransformPig::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	case Client::CTransformPig::STATE_DIE:
		Tick_Die(fTimeDelta);
		break;
	case Client::CTransformPig::STATE_ATTACK:
		Tick_Attack(fTimeDelta);
		break;
	}
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < -10)
	{
		CSpawnerManager::Get_Instance()->Check_MonsterIndex(m_iIndexNum);
		Set_Dead();
	}

	Safe_Release(pGameInstance);
}
void CTransformPig::LateTick(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		if (m_eCurState != STATE_DIE)
			SetState(STATE_CHASE, m_eDir);


	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	if (m_iHp > 0)
	{
		m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}
HRESULT CTransformPig::Render()
{
	Set_Billboard();

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





void CTransformPig::Tick_Idle(_float fTimeDelta)
{
	m_iMove = CGameInstance::Get_Instance()->Get_Random(0, 1000);

	switch (m_iMove)
	{
	case MOVE_R:
		SetState(STATE_MOVE, DIR_R);
		m_fEndPos.x = m_fStartPos.x + m_fDistance;
		break;
	case MOVE_L:
		SetState(STATE_MOVE, DIR_L);
		m_fEndPos.x = m_fStartPos.x - m_fDistance;
		break;
	case MOVE_U:
		SetState(STATE_MOVE, DIR_R);
		m_fEndPos.z = m_fStartPos.z + m_fDistance;
		break;
	case MOVE_D:
		SetState(STATE_MOVE, DIR_L);
		m_fEndPos.z = m_fStartPos.z - m_fDistance;
		break;
	}
}
void CTransformPig::Tick_Move(_float fTimeDelta)
{
	_float3 fPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_iMove == MOVE_R)
	{
		if (m_fEndPos.x <= fPos.x)		// 최종 거리가 이동 거리보다 작거나 같아지면
			m_bDir = true;
		else if (m_fStartPos.x > fPos.x)
			m_bDir = false;

		if (!m_bDir)
		{
			m_pTransformCom->Go_Right(fTimeDelta);
			SetState(STATE_MOVE, DIR_R);
		}
		else
		{
			m_pTransformCom->Go_Left(fTimeDelta);
			SetState(STATE_MOVE, DIR_L);
		}
	}
	else if (m_iMove == MOVE_L)
	{
		if (m_fEndPos.x >= fPos.x)		// 최종 거리가 이동 거리보다 작거나 같아지면
			m_bDir = true;
		else if (m_fStartPos.x < fPos.x)
			m_bDir = false;

		if (!m_bDir)
		{
			m_pTransformCom->Go_Left(fTimeDelta);
			SetState(STATE_MOVE, DIR_L);
		}
		else
		{
			m_pTransformCom->Go_Right(fTimeDelta);
			SetState(STATE_MOVE, DIR_R);
		}
	}
	else if (m_iMove == MOVE_U)
	{
		if (m_fEndPos.z <= fPos.z)		// 최종 거리가 이동 거리보다 작거나 같아지면
			m_bDir = true;
		else if (m_fStartPos.z > fPos.z)
			m_bDir = false;

		if (!m_bDir)
		{
			m_pTransformCom->Go_U(fTimeDelta);
			SetState(STATE_MOVE, DIR_R);
		}
		else
		{
			m_pTransformCom->Go_D(fTimeDelta);
			SetState(STATE_MOVE, DIR_L);
		}
	}
	else if (m_iMove == MOVE_D)
	{
		if (m_fEndPos.z >= fPos.z)		// 최종 거리가 이동 거리보다 작거나 같아지면
			m_bDir = true;
		else if (m_fStartPos.z < fPos.z)
			m_bDir = false;

		if (!m_bDir)
		{
			m_pTransformCom->Go_D(fTimeDelta);
			SetState(STATE_MOVE, DIR_R);
		}
		else
		{
			m_pTransformCom->Go_U(fTimeDelta);
			SetState(STATE_MOVE, DIR_L);
		}
	}
}
void CTransformPig::Tick_Hit(_float fTimeDelta)
{
}

void CTransformPig::Tick_Chase(_float fTimeDelta)
{
	if (GetKeyState('L') & 0x8000)
	{
		SetState(STATE_JUMP, m_eDir);
	}


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < vPlayerPos.x)
	{
		SetState(STATE_CHASE, DIR_R);
		m_pTransformCom->Chase(vPlayerPos + _float3(-0.1f, 0.f, 0.1f), fTimeDelta);
	}
	else
	{
		SetState(STATE_CHASE, DIR_L);
		m_pTransformCom->Chase(vPlayerPos + _float3(0.1f, 0.f, 0.1f), fTimeDelta);
	}

	Safe_Release(pGameInstance);
}

void CTransformPig::Tick_Die(_float fTimeDelta)
{
	m_fCountDead += fTimeDelta;
	if (m_fCountDead >= 2.5f)
		Set_Dead();
}

void CTransformPig::Tick_Attack(_float fTimeDelta)
{
	m_fCountAttack += fTimeDelta;
	if (m_fCountAttack > 1.4f)
	{
		m_fCountAttack = 0;
		if (m_eDir == DIR_R)
			SetState(STATE_CHASE, DIR_R);
		else
			SetState(STATE_CHASE, DIR_L);
	}
}




void CTransformPig::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();

	if (m_eCurState == STATE_JUMP)
	{
		// *중력 코드
		// Y축 힘을 줍니다.
		m_pTransformCom->Set_Vel(4.0f);
	}
}
void CTransformPig::SetAni()
{
	switch (m_eCurState)
	{
	case CTransformPig::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_Idle"), 0.2f, CAnimator::STATE_LOOF);
		break;
	case CTransformPig::STATE_MOVE:
	{
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_MoveR"), 0.2f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_Move"), 0.2f, CAnimator::STATE_LOOF);
	}
	break;
	case CTransformPig::STATE_HIT:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_HitR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_Hit"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case CTransformPig::STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CTransformPig::STATE_DIE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_DieR"), 0.15f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_Die"), 0.15f, CAnimator::STATE_ONCE);
		break;
	case CTransformPig::STATE_ATTACK:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_AttackR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_TransformPig_Attack"), 0.1f, CAnimator::STATE_ONCE);
		break;
	}
}

void CTransformPig::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_eCurState != STATE_DIE)
	{
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < vPlayerPos.x)
			SetState(STATE_HIT, DIR_R);
		else
			SetState(STATE_HIT, DIR_L);
	}
	if (pOther->Get_Tag() == "Player_Skill")
	m_iHp -= pOther->Get_Damage();
	if (m_iHp <= 0)
	{
		//CQuestManager::Get_Instance()->Eat_Item(TEXT("TransformPig"));
		//CSpawnerManager::Get_Instance()->Check_MonsterIndex(m_iIndexNum);
		CToolManager::Get_Instance()->DecreaseMonCount();
		Die();
		CUIManager::Get_Instance()->Set_PlayerExp(20);
	}

	Safe_Release(pGameInstance);
}






CTransformPig * CTransformPig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransformPig*		pInstance = new CTransformPig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTransformPig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CTransformPig::Clone(void* pArg)
{
	CTransformPig*		pInstance = new CTransformPig(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTransformPig"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CTransformPig::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Cube")
	{
		if (m_eCurState == STATE_JUMP)
		{
			if (Get_PushedY())
				SetState(STATE_IDLE, m_eDir);
		}
		//m_pTransformCom->Set_Vel(0.f);
	}
}




void CTransformPig::Free()
{
	__super::Free();

}


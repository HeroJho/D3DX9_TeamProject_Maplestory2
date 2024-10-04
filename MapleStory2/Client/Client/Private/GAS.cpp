#include "stdafx.h"
#include "..\Public\GAS.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "UIManager.h"
#include "BlueMushmom.h"
#include "QuestManager.h"
#include "Spawner.h"

#include "CutSceneManager.h"
#include "ParticleManager.h"
#include "Potal.h"
#include "ToolManager.h"

CGAS::CGAS(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CGAS::CGAS(const CGAS & rhs)
	: CCreature(rhs)
{
}




HRESULT CGAS::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CGAS::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_sTag = "Tag_Monster";
	m_iHp = 50;
	m_iIndexNum = 0;
	m_fCountDead = 0;
	m_iDamage = 1;
	m_iRandomPattern = 0;

	m_bPatterStart = false;

	m_fPatternCycle = 4;

	m_fJump = 0;
	m_fDJump = 0;
	m_fAttack = 0;
	m_fDash = 0;
	m_fEnd = 0;
	m_fReset = 0;
	m_fReturn = 0;

	//모든 패턴을 실행시키기 위한 변수
	m_iFirstPattern = -1;

	m_iFirstSlime = -1;

	m_iSlimePattern = 0;

	m_bSlime = false;
	m_bTest = false;
	m_bVanish = false;
	m_bEffect = false;
	m_bFirstPattern = false;

	m_fColRad = 1.f;
	m_pTransformCom->Set_Scaled(10.f);



	if (nullptr != pArg)
	{
		GASDESC* pDesc = (GASDESC*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
		SetState(pDesc->eState, DIR_END);
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(5.f, 0.f, -5.f));
		SetState(STATE_IDLE, DIR_END);
	}


	// SetShadow(LEVEL_GAS, 4.5f);

	return S_OK;
}




HRESULT CGAS::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 3.f, 5.f, 3.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_MoveR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Hit"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_HitR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Die"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_DieR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Attack"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_AttackR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Jump"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_JumpR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_DJump"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_DJumpR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_TP"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_TPR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_TPA"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_TPAR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Idle"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Skill"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Vanish"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_VanishR"), nullptr);

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_Return"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GAS_ReturnR"), nullptr);
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

void CGAS::Die()
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





void CGAS::Tick(_float fTimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (STATE_CUTSCEEN != m_eCurState && STATE_CUTSCEEN_2 != m_eCurState)
	{
		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

		_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

		if (fabs(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x - vPlayerPos.x) < 1.f)
			m_bPatterStart = true;


		CUIManager::Get_Instance()->Get_GASHp(m_iHp);

		if (m_bPatterStart && m_eCurState != STATE_RESET)
			m_fPatternCycle += fTimeDelta;

		if (m_fPatternCycle > 7.f)
		{
			m_bAttackSound = false;
			m_bDjumpSound = false;
			m_bJumpSound = false;
			m_bPatternSound = false;
			m_bDashSound = false;
			m_bReset = false;
			m_bReturn = false;

			m_fPatternCycle = 0;
			if (!m_bFirstPattern)
			{
				m_iFirstPattern++;
				m_iRandomPattern = m_iFirstPattern;
				if (m_iFirstPattern >= 5 && m_bSlime)
					m_bFirstPattern = true;
				else if (m_iFirstPattern >= 5 && !m_bSlime)
					m_iRandomPattern = 5;
			}
			else
			{
				m_iRandomPattern = CGameInstance::Get_Instance()->Get_Random(1, 5);
			}
		}

		vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

		if (fabs(m_pTransformCom->Get_State(CTransform::STATE_POSITION).x - vPlayerPos.x) < 1.f)
			m_bPatterStart = true;

		if (m_eCurState != STATE_HIT && m_eCurState != STATE_DIE)
		{

			if (m_iRandomPattern == 0)
			{
				if (vPlayerPos.x > m_pTransformCom->Get_State(CTransform::STATE_POSITION).x)
					SetState(STATE_CHASE, DIR_R);
				else
					SetState(STATE_CHASE, DIR_L);
			}

			else if (m_iRandomPattern == 3) // 점프패턴
			{
				if (m_eDir == DIR_R)
					SetState(STATE_JUMP, DIR_R);
				else
					SetState(STATE_JUMP, DIR_L);
			}

			else if (m_iRandomPattern == 2) // 일반공격
			{
				if (m_eDir == DIR_R)
					SetState(STATE_ATTACK, DIR_R);
				else
					SetState(STATE_ATTACK, DIR_L);
			}

			else if (m_iRandomPattern == 1) // 텔레포트
			{
				if (m_eDir == DIR_R)
					SetState(STATE_DASH, DIR_R);
				else
					SetState(STATE_DASH, DIR_L);
			}

			else if (m_iRandomPattern == 4) // 슬라임 소환
			{
				if (m_eDir == DIR_R)
					SetState(STATE_DJUMP, DIR_R);
				else
					SetState(STATE_DJUMP, DIR_L);
			}


			else if (m_iRandomPattern == 100)	// 텔포 이후
			{
				if (vPlayerPos.x > m_pTransformCom->Get_State(CTransform::STATE_POSITION).x)
					SetState(STATE_END, DIR_R);
				else
					SetState(STATE_END, DIR_L);
			}

			else if (m_iRandomPattern == 5)	// 게이트 소환
			{
				if (m_eDir == DIR_R)
					SetState(STATE_RESET, DIR_R);
				else
					SetState(STATE_RESET, DIR_L);
			}

			else if (m_iRandomPattern == 6)	// 재생성
			{
				if (m_eDir == DIR_R)
					SetState(STATE_RETURN, DIR_R);
				else
					SetState(STATE_RETURN, DIR_L);
			}

		}
	}

	Safe_Release(pGameInstance);


	switch (m_eCurState)
	{
	case STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	case STATE_DASH:
		Tick_Dash(fTimeDelta);
		break;
	case STATE_JUMP:
		Tick_Jump(fTimeDelta);
		break;
	case STATE_DJUMP:
		Tick_DJump(fTimeDelta);
		break;
	case STATE_ATTACK:
		Tick_Attack(fTimeDelta);
		break;
	case STATE_DIE:
		Tick_Die(fTimeDelta);
		break;
	case STATE_CUTSCEEN:
		Tick_CutScene(fTimeDelta);
		break;
	case STATE_CUTSCEEN_2:
		Tick_CutScene2(fTimeDelta);
		break;
	case STATE_RESET:
		Tick_Reset(fTimeDelta);
		break;
	case STATE_RETURN:
		Tick_Return(fTimeDelta);
		break;
	case STATE_END:
		Tick_End(fTimeDelta);
		break;
	}


}

void CGAS::LateTick(_float fTimeDelta)
{

	m_iColliTime += fTimeDelta;

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	_float3 Mushpos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (Mushpos.y < -1.f)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(Mushpos.x, 3.5f, Mushpos.z));

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		if (m_eCurState != STATE_DIE && m_eCurState != STATE_ATTACK && m_eCurState != STATE_RESET)
			SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	if (m_iHp > 0 && m_eCurState != STATE_RESET && m_iColliTime > 10.f)
	{
		m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	}

	Set_Billboard();
}

HRESULT CGAS::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;



	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	//if (FAILED(Set_RenderState()))
	//	return E_FAIL;



	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}


void CGAS::Tick_CutScene(_float fTimeDelta)
{
	if (!m_bCutSceneRend)
		return;

	m_fCutSceneTimeAcc += fTimeDelta;

	CGameObject* pJang = CCutSceneManager::Get_Instance()->Get_Jang();
	CTransform* pJangTran = (CTransform*)pJang->Get_ComponentPtr(TEXT("Com_Transform"));
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos.y -= 1.8f;
	vPos.x -= 0.5f;
	vPos.z -= 0.8f;
	pJangTran->Set_State(CTransform::STATE_POSITION, vPos);


	_float3 vPosa = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (2.f < m_fCutSceneTimeAcc && !m_bCutSceneJump)
	{
		CGameInstance::Get_Instance()->PlaySound(L"Bird.wav", 10, 1.f);
		m_pTransformCom->Set_Vel(30.f);
		m_bCutSceneJump = true;
		CCutSceneManager::Get_Instance()->Set_JangRander(false);
		CGameInstance::Get_Instance()->PlaySound(L"MushJump.mp3", 10, 1.f);
	}
	else if (vPosa.y > 10.f && m_bCutSceneJump)
	{
		Set_Dead();
	}

}
void CGAS::Tick_CutScene2(_float fTimeDelta)
{
	m_fCutSceneTimeAcc += fTimeDelta;


	if (10.f < m_fCutSceneTimeAcc && !m_bCutSceneJump)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3{ 0.f, 3.f, 9.f });
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_TPA"), 0.1f, CAnimator::STATE_ONCE);
		CGameInstance::Get_Instance()->PlaySound(L"GAStp.wav", 16, 1.f);
		m_bCutSceneJump = true;
	}
}

void CGAS::Tick_Idle(_float fTimeDelta)
{
}
void CGAS::Tick_Hit(_float fTimeDelta)
{
}

void CGAS::Tick_Chase(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < vPlayerPos.x)
		SetState(STATE_CHASE, DIR_R);
	else
		SetState(STATE_CHASE, DIR_L);


	m_pTransformCom->Chase(vPlayerPos + _float3(0.1f, 0, 0), fTimeDelta * 3);

	Safe_Release(pGameInstance);
}

void CGAS::Tick_Jump(_float fTimeDelta)
{
	m_bShake = true;
	m_fJump += fTimeDelta;
	if (m_fJump > 1.4f && !m_bJumpSound)
	{
		CGameInstance::Get_Instance()->PlaySound(L"GASjump.wav", 10, 0.7f);
		m_bJumpSound = true;

		CGameInstance::Get_Instance()->PlaySound(L"GASjump.wav", 10, 0.7f);

		CCutSceneManager::Get_Instance()->Get_MainCam()->Start_AttackShaking();
		CParticleManager::Get_Instance()->BlueMushRoom_Lend(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_bShake = false;


		CCreature* pPlayer = (CCreature*)CToolManager::Get_Instance()->GetPlayer();
		CTransform* pPlayerTran = (CTransform*)pPlayer->Get_ComponentPtr(TEXT("Com_Transform"));
		_float3 PlayerPos = pPlayerTran->Get_State(CTransform::STATE_POSITION);
		_float3 MePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_float PlayerY = pPlayerTran->Get_Vel();
		if (-0.1f < PlayerY && 0.1f > PlayerY)
		{
			_float3 vDir = PlayerPos - MePos;
			vDir.y = 0.f;
			pPlayer->SetKnockBack(7.f, 8.f, vDir);
		}
	}

	if (m_fJump > 2.0f)
	{
		m_iRandomPattern = 0;
		m_fJump = 0;

	}
}

void CGAS::Tick_DJump(_float fTimeDelta)
{
	m_fDJump += fTimeDelta;

	if (m_fDJump > 1.8f && !m_bDjumpSound)
	{
		CGameInstance::Get_Instance()->PlaySound(L"GASDjump.wav", 16, 1.f);
		m_bDjumpSound = true;
	}


	if (m_fDJump > 2.f && !m_bTest)
	{
		m_bTest = true;

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CSpawner::SPAWNERINFO m_Slime;


		m_Slime.GAS = TEXT("GAS");
		m_Slime.Level = LEVEL_GAS;
		m_Slime.MonsterColRad = 1.f;

		m_Slime.MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		float random = pGameInstance->Get_FloatRandom(-2, 2);
		float random2 = pGameInstance->Get_FloatRandom(-2, 2);
		float random3 = pGameInstance->Get_FloatRandom(0, 2);

		m_Slime.MonsterPos += _float3{ random, random3, random2 };

		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Slime"), LEVEL_GAS, TEXT("Layer_Monster"), &m_Slime)))
			int a = 0;


		for (int i = 0; i < 7; ++i)
		{
			m_Slime.MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			random = pGameInstance->Get_FloatRandom(-5, 5);
			random2 = pGameInstance->Get_FloatRandom(-5, 5);
			random3 = pGameInstance->Get_FloatRandom(3, 7);

			m_Slime.MonsterPos += _float3{ random, random3, random2 };

			if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Slime"), LEVEL_GAS, TEXT("Layer_Monster"), &m_Slime)))
				int a = 0;
		}

		//for (int i = 0; i < 4; ++i)
		//{
		//	m_Slime.MonsterNum = i;
		//	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_PurpleSlime"), LEVEL_GAS, TEXT("Layer_Monster"), &m_Slime)))
		//		int a = 0;
		//}

		Safe_Release(pGameInstance);
	}
	if (m_fDJump > 2.2f)
	{
		m_iRandomPattern = 0;
		m_fDJump = 0;
		m_bTest = false;
	}
}

void CGAS::Tick_Dash(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	m_fDash += fTimeDelta;


	if (m_fDash > 0.1f && !m_bDashSound)
	{
		CGameInstance::Get_Instance()->PlaySound(L"GAStp.wav", 16, 1.f);
		m_bDashSound = true;
	}

	if (m_fDash > 1.6f)
	{
		m_iRandomPattern = 100;
		m_fDash = 0;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPlayerPos + _float3(0.f, 1.f, 0.f));
	}

	Safe_Release(pGameInstance);
}

void CGAS::Tick_End(_float fTimeDelta)
{
	m_fEnd += fTimeDelta;
	if (m_fEnd > 0.8f)
	{
		m_iRandomPattern = 0;
		m_fEnd = 0;
	}
}

void CGAS::Tick_Attack(_float fTimeDelta)
{
	m_fAttack += fTimeDelta;

	if (m_fAttack > 0.1f && !m_bAttackSound)
	{
		CGameInstance::Get_Instance()->PlaySound(L"GAScharging.wav", 16, 1.f);
		m_bAttackSound = true;
	}


	if (m_fAttack > 1.9f && !m_bEffect)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CSpawner::SPAWNERINFO m_Effect;

		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

		_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

		m_Effect.MonsterPos = vPlayerPos + _float3(0.f, 0.f, 0.1f);

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GASEffect"), LEVEL_GAS, TEXT("Layer_Player_Skill"), &m_Effect);

		m_bEffect = true;

		Safe_Release(pGameInstance);
	}

	if (m_fAttack > 2.0f)
	{
		m_iRandomPattern = 0;
		m_fAttack = 0;
		m_bEffect = false;
	}
}


void CGAS::Tick_Die(_float fTimeDelta)
{
	m_fCountDead += fTimeDelta;
	if (m_fCountDead >= 8.0f)
	{
		//CGameInstance::Get_Instance()->PlaySound(L"GASpattern.wav", 16, 1.f);
		Set_Dead();
	}
}

void CGAS::Tick_Reset(_float fTimeDelta)
{
	m_fReset += fTimeDelta;

	if (m_fReset > 0.1f && !m_bPatternSound)
	{
		CGameInstance::Get_Instance()->PlaySound(L"GAS4slime.wav", 16, 1.f);
		m_bPatternSound = true;
	}

	if (!m_bVanish && m_fReset > 2.1f)
	{

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CSpawner::SPAWNERINFO m_Slime;

		MakeBlockCube(_float3(-5.5f, 4.5f, 2.f));
		MakeBlockCube(_float3(5.5f, 4.5f, 2.f));

		if (!m_bSlime)
		{
			m_iFirstSlime++;
			m_iSlimePattern = m_iFirstSlime;
			if (m_iFirstSlime >= 3)
				m_bSlime = true;
		}
		else
		{
			m_iFirstSlime = CGameInstance::Get_Instance()->Get_Random(0, 3);
		}

		switch (m_iSlimePattern)
		{
		case 0:
			for (int i = 0; i < 4; ++i)
			{
				m_Slime.MonsterNum = i;
				pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_RedSlime"), LEVEL_GAS, TEXT("Layer_Monster"), &m_Slime);
			}
			break;
		case 1:
			for (int i = 0; i < 4; ++i)
			{
				m_Slime.MonsterNum = i;
				pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GreenSlime"), LEVEL_GAS, TEXT("Layer_Monster"), &m_Slime);
			}
			break;
		case 2:
			for (int i = 0; i < 4; ++i)
			{
				m_Slime.MonsterNum = i;
				pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlueSlime"), LEVEL_GAS, TEXT("Layer_Monster"), &m_Slime);
			}
			break;
		case 3:
			for (int i = 0; i < 4; ++i)
			{
				m_Slime.MonsterNum = i;
				pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_PurpleSlime"), LEVEL_GAS, TEXT("Layer_Monster"), &m_Slime);
			}
			break;
		}
		m_bVanish = true;

		Safe_Release(pGameInstance);
	}

	if (m_fReset > 15.5f)
	{
		m_iRandomPattern = 6;
		m_fReset = 0;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 5.f, 0.f));
		m_bVanish = false;
	}
}

void CGAS::Tick_Return(_float fTimeDelta)
{
	m_fReturn += fTimeDelta;

	if (m_fReturn > 0.1f && !m_bReturn)
	{
		CGameInstance::Get_Instance()->PlaySound(L"GAS4slimeReturn.wav", 16, 1.f);
		m_bReturn = true;
	}

	if (m_fReturn > 2.1f)
	{
		m_iRandomPattern = 0;
		if (m_eDir == DIR_R)
			SetState(STATE_CHASE, DIR_R);
		else
			SetState(STATE_CHASE, DIR_L);
		m_fReturn = 0;
	}
}


void CGAS::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CGAS::SetAni()
{
	switch (m_eCurState)
	{
	case STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Idle"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case STATE_MOVE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case STATE_HIT:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_HitR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Hit"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case STATE_DIE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_DieR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Die"), 0.1f, CAnimator::STATE_ONCE);
		break;
	case STATE_JUMP:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_JumpR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Jump"), 0.1f, CAnimator::STATE_ONCE);
		break;

	case STATE_ATTACK:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_AttackR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Attack"), 0.1f, CAnimator::STATE_ONCE);
		break;

	case STATE_DASH:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_TPR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_TP"), 0.1f, CAnimator::STATE_ONCE);
		break;

	case STATE_DJUMP:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_DJumpR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_DJump"), 0.1f, CAnimator::STATE_ONCE);
		break;

	case STATE_CUTSCEEN:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Idle"), 0.1f, CAnimator::STATE_LOOF);
		break;

	case STATE_CUTSCEEN_2:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Idle"), 0.1f, CAnimator::STATE_LOOF);
		break;

	case STATE_END:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_TPAR"), 0.15f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_TPA"), 0.15f, CAnimator::STATE_ONCE);
		break;

	case STATE_RESET:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_VanishR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Vanish"), 0.1f, CAnimator::STATE_ONCE);
		break;

	case STATE_RETURN:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_ReturnR"), 0.1f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GAS_Return"), 0.1f, CAnimator::STATE_ONCE);
		break;
	}
}

void CGAS::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_eCurState == STATE_CHASE || m_eCurState == STATE_IDLE)
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
		//0.f, 2.5f, 18.f
		CQuestManager::Get_Instance()->Hunting(TEXT("GAS"));

		CGameInstance::Get_Instance()->PlaySound(L"SlimeDie.wav", 13, 1.f);

		CPotal::POTALDESC PotalDesc;
		PotalDesc.eDestLevel = LEVEL_GAMEPLAY;
		PotalDesc.Pos = _float3(0.f, 2.5f, 17.5f);
		PotalDesc.DestPos = _float3(9.f, 9.f, -1.5f);
		(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Potal"), LEVEL_GAS, L"Layer_Potal", &PotalDesc));

		CCutSceneManager::Get_Instance()->Set_JangRander(true);

		Die();
		CUIManager::Get_Instance()->Set_PlayerExp(180);
	}

	Safe_Release(pGameInstance);
}

void CGAS::DestroyCube(_int iLength)
{
	_float4x4 Matrix;
	_float3 foriDir = { 1.f, 0.f, 0.f };
	_float3 fDir;
	_float3 vPos;
	for (_int i = 0; i <= 360; i += 45)
	{
		_int iAngle = i + 20;
		D3DXMatrixRotationY(&Matrix, D3DXToRadian(_float(iAngle)));
		D3DXVec3TransformNormal(&fDir, &foriDir, &Matrix);
		vPos = fDir * iLength;
		CMap_Manager::CUBEDATA desc;
		vPos.y += 2.f;
		desc.vPos = vPos;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Trigger"), LEVEL_GAS, TEXT("Layer_Cube"), &desc)))
			return;
	}

}

void CGAS::MakeBlockCube(_float3 vPos)
{
	CMap_Manager::CUBEDATA desc;
	desc.iIndex = 1;
	desc.vPos = vPos;

	if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_PushCube"), LEVEL_GAS, TEXT("Layer_Cube"), &desc)))
		return;
}


CGAS * CGAS::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGAS*		pInstance = new CGAS(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGAS"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CGAS::Clone(void* pArg)
{
	CGAS*		pInstance = new CGAS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGAS"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CGAS::Collision(CGameObject * pOther)
{
	if (m_iHp <= 0)
		return;



	if (STATE_CUTSCEEN == m_eCurState)
	{
		if ("Tag_Cube" == pOther->Get_Tag() && !m_bCutSceneRend)
		{
			CCutSceneManager::Get_Instance()->Get_MainCam()->Start_AttackShaking();
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos.x += 0.1;
			vPos.z -= 0.1;
			vPos.y -= 0.5;
			CParticleManager::Get_Instance()->BlueMushRoom_Lend(vPos);
			CGameInstance::Get_Instance()->PlaySoundW(L"MushDown.mp3", 16, 1.f);
			m_pTransformCom->Set_Vel(3.f);
			m_bCutSceneRend = true;
		}
	}
}


void CGAS::Free()
{
	__super::Free();

}


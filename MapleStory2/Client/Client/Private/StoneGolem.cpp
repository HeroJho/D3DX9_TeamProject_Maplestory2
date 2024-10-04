#include "stdafx.h"
#include "..\Public\StoneGolem.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "SpawnerManager.h"

CStoneGolem::CStoneGolem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CStoneGolem::CStoneGolem(const CStoneGolem & rhs)
	: CCreature(rhs)
{
}




HRESULT CStoneGolem::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CStoneGolem::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Monster";
	m_iHp = 10;
	m_iIndexNum = -1;


	m_fColRad = 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(43.f, 10.f, -7.f));
	m_pTransformCom->Set_Scaled(6.f);

	SetState(STATE_IDLE, DIR_END);

	SetShadow(LEVEL_HENESYS, 6.f);


	return S_OK;
}




HRESULT CStoneGolem::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 4.f, 3.5f, 4.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StoneGolem_Idle"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StoneGolem_Hit"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StoneGolem_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StoneGolem_HitR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StoneGolem_MoveR"), nullptr);
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




void CStoneGolem::Tick(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case Client::CStoneGolem::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CStoneGolem::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CStoneGolem::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CStoneGolem::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < -10)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(43.f, 10.f, -7.f));
	}

}
void CStoneGolem::LateTick(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER, this);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	Set_Billboard();
}
HRESULT CStoneGolem::Render()
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





void CStoneGolem::Tick_Idle(_float fTimeDelta)
{
}
void CStoneGolem::Tick_Move(_float fTimeDelta)
{
}
void CStoneGolem::Tick_Hit(_float fTimeDelta)
{
}

void CStoneGolem::Tick_Chase(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < vPlayerPos.x)
		SetState(STATE_CHASE, DIR_R);
	else
		SetState(STATE_CHASE, DIR_L); 


	m_pTransformCom->Chase(vPlayerPos, fTimeDelta);

	Safe_Release(pGameInstance);
}



void CStoneGolem::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CStoneGolem::SetAni()
{
	switch (m_eCurState)
	{
	case CStoneGolem::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_StoneGolem_Idle"), 1.f, CAnimator::STATE_LOOF);
	break;
	case CStoneGolem::STATE_MOVE:
	{

	}
	break;
	case CStoneGolem::STATE_HIT:
		if(m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_StoneGolem_HitR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_StoneGolem_Hit"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case CStoneGolem::STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_StoneGolem_MoveR"), 1.f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_StoneGolem_Move"), 1.2f, CAnimator::STATE_LOOF);
		break;
	}
}

void CStoneGolem::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x < vPlayerPos.x)
		SetState(STATE_HIT, DIR_R);
	else
		SetState(STATE_HIT, DIR_L);


	Safe_Release(pGameInstance);

	--m_iHp;
	if (m_iHp == 0)
	{
		CQuestManager::Get_Instance()->Hunting(TEXT("StoneGolem"));
		//CSpawnerManager::Get_Instance()->Check_MonsterIndex(m_iIndexNum);
		Set_Dead();
	}

}






CStoneGolem * CStoneGolem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStoneGolem*		pInstance = new CStoneGolem(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CStoneGolem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CStoneGolem::Clone(void* pArg)
{
	CStoneGolem*		pInstance = new CStoneGolem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CStoneGolem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CStoneGolem::Collision(CGameObject * pOther)
{

}




void CStoneGolem::Free()
{
	__super::Free();

}


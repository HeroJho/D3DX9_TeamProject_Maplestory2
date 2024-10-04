#include "stdafx.h"
#include "..\Public\RedSlime.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "SpawnerManager.h"

CRedSlime::CRedSlime(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CRedSlime::CRedSlime(const CRedSlime & rhs)
	: CCreature(rhs)
{
}




HRESULT CRedSlime::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CRedSlime::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_GASMonster";
	m_iHp = 10;
	m_iIndexNum = -1;

	CSpawner::SPAWNERINFO* pMonsterDesc = (CSpawner::SPAWNERINFO*)pArg;

	m_iDirection = pMonsterDesc->MonsterNum;

	m_fColRad = 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 2.f, 0.f));
	m_pTransformCom->Set_Scaled(1.f);

	SetState(STATE_CHASE, DIR_L);

	SetShadow(LEVEL_GAS, 1.2f);


	return S_OK;
}




HRESULT CRedSlime::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.3f, 0.6f, 0.3f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RedSlime_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RedSlime_MoveR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RedSlime_DieR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RedSlime_Die"), nullptr);
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




void CRedSlime::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCreature::CRETUREDESC D;

	//_float a = CGameInstance::Get_Instance()->Get_FloatRandom(-8, 8);
	//_float b = CGameInstance::Get_Instance()->Get_FloatRandom(-8, 8);

	//D.vPos = _float3(0.f + a, 2.f, 0.f + b);

	switch (m_iDirection)
	{
	case R:
		m_pTransformCom->Chase(_float3(-6.5f, 0.f, 7.7f), fTimeDelta * 2.5f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_Move"), 0.1f, CAnimator::STATE_LOOF);



		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x <= -6.4f)
		{
			for (int i = 0; i < 3; i++)
			{
				_float a = CGameInstance::Get_Instance()->Get_FloatRandom(-8, 8);
				_float b = CGameInstance::Get_Instance()->Get_FloatRandom(-8, 8);

				D.vPos = _float3(0.f + a, 2.f, 0.f + b);
				(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_RedSkill"), LEVEL_GAS, TEXT("Layer_NPC"), &D));
			}
			Set_Dead();
		}
		break;
	case G:
		m_pTransformCom->Chase(_float3(6.5f, 0.f, 7.7f), fTimeDelta * 2.5f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >= 6.4f)
		{
			Set_Dead();
		}
		break;
	case B:
		m_pTransformCom->Chase(_float3(10.5f, 0.f, 0.f), fTimeDelta * 2.7f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >= 10.4f)
		{
			Set_Dead();
		}
		break;
	case P:
		m_pTransformCom->Chase(_float3(-10.5f, 0.f, 0.f), fTimeDelta * 2.7f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x <= -10.4f)
		{
			Set_Dead();
		}
		break;
	}

	Safe_Release(pGameInstance);

}
void CRedSlime::LateTick(_float fTimeDelta)
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
HRESULT CRedSlime::Render()
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





void CRedSlime::Tick_Idle(_float fTimeDelta)
{
}
void CRedSlime::Tick_Move(_float fTimeDelta)
{
}
void CRedSlime::Tick_Hit(_float fTimeDelta)
{
}

void CRedSlime::Tick_Chase(_float fTimeDelta)
{
}



void CRedSlime::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CRedSlime::SetAni()
{
	switch (m_eCurState)
	{
	case CRedSlime::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
	break;
	case CRedSlime::STATE_MOVE:
	{

	}
	break;
	case CRedSlime::STATE_HIT:
		if(m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_DieR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_Die"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case CRedSlime::STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	}
}

void CRedSlime::Damaged(CGameObject * pOther)
{
}






CRedSlime * CRedSlime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRedSlime*		pInstance = new CRedSlime(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CRedSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CRedSlime::Clone(void* pArg)
{
	CRedSlime*		pInstance = new CRedSlime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CRedSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CRedSlime::Collision(CGameObject * pOther)
{

}




void CRedSlime::Free()
{
	__super::Free();

}


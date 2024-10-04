#include "stdafx.h"
#include "..\Public\GreenSlime.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "SpawnerManager.h"
#include "GAS.h"

CGreenSlime::CGreenSlime(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CGreenSlime::CGreenSlime(const CGreenSlime & rhs)
	: CCreature(rhs)
{
}




HRESULT CGreenSlime::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CGreenSlime::Initialize(void * pArg)
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
	m_pTransformCom->Set_Scaled(1.5f);

	SetState(STATE_CHASE, DIR_L);

	SetShadow(LEVEL_GAS, 1.5f);


	return S_OK;
}




HRESULT CGreenSlime::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.3f, 1.f, 0.3f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenSlime_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenSlime_MoveR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenSlime_DieR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GreenSlime_Die"), nullptr);
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




void CGreenSlime::Tick(_float fTimeDelta)
{


	switch (m_iDirection)
	{
	case R:
		m_pTransformCom->Chase(_float3(-6.5f, 0.f, 7.7f), fTimeDelta * 2.5f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x <= -6.4f)
		{
			Set_Dead();
		}
		break;
	case G:
		m_pTransformCom->Chase(_float3(6.5f, 0.f, 7.7f), fTimeDelta * 2.5f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >= 6.4f)
		{
			CGameInstance* pInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pInstance);
			((CGAS*)pInstance->Get_ObjectPtr(LEVEL_GAS, L"Layer_Monster", 0))->Reset_Hp();
			(pInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_GreenSkill"), LEVEL_GAS, TEXT("Layer_UI")));
			Safe_Release(pInstance);
			Set_Dead();
		}
		break;
	case B:
		m_pTransformCom->Chase(_float3(10.5f, 0.f, 0.f), fTimeDelta * 2.7f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >= 10.4f)
		{
			Set_Dead();
		}
		break;
	case P:
		m_pTransformCom->Chase(_float3(-10.5f, 0.f, 0.f), fTimeDelta * 2.7f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x <= -10.4f)
		{
			Set_Dead();
		}
		break;
	}

}
void CGreenSlime::LateTick(_float fTimeDelta)
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
HRESULT CGreenSlime::Render()
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





void CGreenSlime::Tick_Idle(_float fTimeDelta)
{
}
void CGreenSlime::Tick_Move(_float fTimeDelta)
{
}
void CGreenSlime::Tick_Hit(_float fTimeDelta)
{
}

void CGreenSlime::Tick_Chase(_float fTimeDelta)
{
}



void CGreenSlime::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CGreenSlime::SetAni()
{
	switch (m_eCurState)
	{
	case CGreenSlime::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
	break;
	case CGreenSlime::STATE_MOVE:
	{

	}
	break;
	case CGreenSlime::STATE_HIT:
		if(m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_DieR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_Die"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case CGreenSlime::STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GreenSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	}
}

void CGreenSlime::Damaged(CGameObject * pOther)
{
}






CGreenSlime * CGreenSlime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGreenSlime*		pInstance = new CGreenSlime(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGreenSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CGreenSlime::Clone(void* pArg)
{
	CGreenSlime*		pInstance = new CGreenSlime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGreenSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CGreenSlime::Collision(CGameObject * pOther)
{

}




void CGreenSlime::Free()
{
	__super::Free();

}


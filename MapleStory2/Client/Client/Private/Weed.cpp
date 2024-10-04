#include "stdafx.h"
#include "..\Public\Weed.h"
#include "GameInstance.h"


CWeed::CWeed(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CWeed::CWeed(const CWeed & rhs)
	: CCreature(rhs)
{
}

HRESULT CWeed::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CWeed::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 0.3f;	
	
	CCreature::CRETUREDESC pNpcDesc = *(CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pNpcDesc.vPos);
	m_pTransformCom->Set_Scaled(0.5f);
	m_fTime = 0.f;
	m_fShaketime = 0.f;
	m_bShake = false;
	m_bCollision = true;
	SetState(STATE_IDLE, DIR_L);
	 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	return S_OK;
}

HRESULT CWeed::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Weed"), nullptr);
		


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CWeed::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{

	case Client::CWeed::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CWeed::STATE_MOVE:
		Tick_Move(fTimeDelta);
		
		break;

	case Client::CWeed::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CWeed::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CWeed::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CWeed::Render()
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
void CWeed::Tick_Idle(_float fTimeDelta)
{	
	if (!m_bCollision)
		m_fTime += fTimeDelta;

	if (m_fTime > 5.f)
		m_bCollision = true;
}
void CWeed::Tick_Stand(_float fTimeDelta)
{
}
void CWeed::Tick_Move(_float fTimeDelta)
{ 
	m_fShaketime += fTimeDelta;
	if (m_fShaketime > 0.6f)
	{
		m_fShaketime = 0.f;
		m_bCollision = false;
		SetState(STATE_IDLE, DIR_L);
		return;
	}
	
		if (m_bShake)
		{
			m_fTime += 1.f;
			vPos.x += 0.03f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			if (m_fTime > 3.f)
			{
				m_bShake = false;
				m_fTime = 0.f;
			}

		}
		else
		{
			m_fTime += 1.f;
			vPos.x -= 0.03f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			if (m_fTime > 3.f)
			{
				m_bShake = true;
				m_fTime = 0.f;
			
			}
		}

	}



void CWeed::Tick_Hit(_float fTimeDelta)
{
}

void CWeed::Tick_Chase(_float fTimeDelta)
{
}



void CWeed::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CWeed::SetAni()
{
	switch (m_eCurState)
	{
	case CWeed::STATE_MOVE:
		CGameInstance::Get_Instance()->PlaySound(L"Leaf.mp3", 8, 1.f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Weed"), 0.2f, CAnimator::STATE_LOOF);
		break;
	case CWeed::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Weed"), 0.2f, CAnimator::STATE_LOOF);
		break;
		
	case CWeed::STATE_HIT:
		break;

	case CWeed::STATE_CHASE:
		break;
	}
}

void CWeed::Damaged(CGameObject * pOther)
{

}

CWeed * CWeed::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeed*		pInstance = new CWeed(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWeed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CWeed::Clone(void* pArg)
{
	CWeed*		pInstance = new CWeed(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CWeed"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CWeed::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Player"&& m_bCollision == true)
	{
		SetState(STATE_MOVE, DIR_L);
		
	}
}




void CWeed::Free()
{
	__super::Free();

}


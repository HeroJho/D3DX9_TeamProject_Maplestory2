#include "stdafx.h"
#include "..\Public\RockEnter.h"
#include "GameInstance.h"


CRockEnter::CRockEnter(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CRockEnter::CRockEnter(const CRockEnter & rhs)
	: CCreature(rhs)
{
}

HRESULT CRockEnter::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CRockEnter::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	
	//CCreature::CRETUREDESC* pRockEnterDesc = (CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(27.f, 29.7f, 6.7f));
	m_pTransformCom->Set_Scaled(3.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CRockEnter::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RockEnter"), nullptr);
		

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CRockEnter::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CRockEnter::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;
}
void CRockEnter::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{

	case Client::CRockEnter::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CRockEnter::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CRockEnter::STATE_HIT:
		
		Tick_Hit(fTimeDelta);
		break;

	case Client::CRockEnter::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CRockEnter::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	__super::Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CRockEnter::Render()
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
void CRockEnter::Tick_Idle(_float fTimeDelta)
{

}
void CRockEnter::Tick_Stand(_float fTimeDelta)
{

}
void CRockEnter::Tick_Move(_float fTimeDelta)
{

}
void CRockEnter::Tick_Hit(_float fTimeDelta)
{

}

void CRockEnter::Tick_Chase(_float fTimeDelta)
{
}



void CRockEnter::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CRockEnter::SetAni()
{
	switch (m_eCurState)
	{
	case CRockEnter::STATE_MOVE:
		break;
	case CRockEnter::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RockEnter"), 0.2f, CAnimator::STATE_LOOF);

		break;
		
	case CRockEnter::STATE_HIT:
		break;

	case CRockEnter::STATE_CHASE:
		break;
	}
}

void CRockEnter::Damaged(CGameObject * pOther)
{

}

CRockEnter * CRockEnter::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRockEnter*		pInstance = new CRockEnter(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CRockEnter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CRockEnter::Clone(void* pArg)
{
	CRockEnter*		pInstance = new CRockEnter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CRockEnter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CRockEnter::Collision(CGameObject * pOther)
{
	
}




void CRockEnter::Free()
{
	__super::Free();

}


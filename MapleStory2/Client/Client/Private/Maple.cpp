#include "stdafx.h"
#include "..\Public\Maple.h"
#include "GameInstance.h"


CMaple::CMaple(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CMaple::CMaple(const CMaple & rhs)
	: CCreature(rhs)
{
}

HRESULT CMaple::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CMaple::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	
	CCreature::CRETUREDESC* pMapleDesc = (CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pMapleDesc->vPos);
	m_pTransformCom->Set_Scaled(3.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CMaple::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Maple"), nullptr);
		

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CMaple::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CMaple::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;
}
void CMaple::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{

	case Client::CMaple::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CMaple::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CMaple::STATE_HIT:
		
		Tick_Hit(fTimeDelta);
		break;

	case Client::CMaple::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CMaple::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CMaple::Render()
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
void CMaple::Tick_Idle(_float fTimeDelta)
{

}
void CMaple::Tick_Stand(_float fTimeDelta)
{

}
void CMaple::Tick_Move(_float fTimeDelta)
{

}
void CMaple::Tick_Hit(_float fTimeDelta)
{

}

void CMaple::Tick_Chase(_float fTimeDelta)
{
}



void CMaple::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CMaple::SetAni()
{
	switch (m_eCurState)
	{
	case CMaple::STATE_MOVE:
		break;
	case CMaple::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Maple"), 0.2f, CAnimator::STATE_LOOF);

		break;
		
	case CMaple::STATE_HIT:
		break;

	case CMaple::STATE_CHASE:
		break;
	}
}

void CMaple::Damaged(CGameObject * pOther)
{

}

CMaple * CMaple::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMaple*		pInstance = new CMaple(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMaple"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CMaple::Clone(void* pArg)
{
	CMaple*		pInstance = new CMaple(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMaple"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CMaple::Collision(CGameObject * pOther)
{
	
}




void CMaple::Free()
{
	__super::Free();

}


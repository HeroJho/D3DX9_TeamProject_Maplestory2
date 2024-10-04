#include "stdafx.h"
#include "..\Public\Root.h"
#include "GameInstance.h"


CRoot::CRoot(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CRoot::CRoot(const CRoot & rhs)
	: CCreature(rhs)
{
}

HRESULT CRoot::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CRoot::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
	
	CCreature::CRETUREDESC* pRootDesc = (CCreature::CRETUREDESC*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pRootDesc->vPos);
	m_pTransformCom->Set_Scaled(4.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CRoot::SetUp_Components()
{

		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Root"), nullptr);
		

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CRoot::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CRoot::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;
}
void CRoot::Tick(_float fTimeDelta)
{
	
	switch (m_eCurState)
	{

	case Client::CRoot::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CRoot::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CRoot::STATE_HIT:
		
		Tick_Hit(fTimeDelta);
		break;

	case Client::CRoot::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CRoot::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), 45);
	//Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CRoot::Render()
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
void CRoot::Tick_Idle(_float fTimeDelta)
{

}
void CRoot::Tick_Stand(_float fTimeDelta)
{

}
void CRoot::Tick_Move(_float fTimeDelta)
{

}
void CRoot::Tick_Hit(_float fTimeDelta)
{

}

void CRoot::Tick_Chase(_float fTimeDelta)
{
}



void CRoot::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CRoot::SetAni()
{
	switch (m_eCurState)
	{
	case CRoot::STATE_MOVE:
		break;
	case CRoot::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Root"), 0.2f, CAnimator::STATE_LOOF);

		break;
		
	case CRoot::STATE_HIT:
		break;

	case CRoot::STATE_CHASE:
		break;
	}
}

void CRoot::Damaged(CGameObject * pOther)
{

}

CRoot * CRoot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRoot*		pInstance = new CRoot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CRoot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CRoot::Clone(void* pArg)
{
	CRoot*		pInstance = new CRoot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CRoot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CRoot::Collision(CGameObject * pOther)
{
	
}




void CRoot::Free()
{
	__super::Free();

}


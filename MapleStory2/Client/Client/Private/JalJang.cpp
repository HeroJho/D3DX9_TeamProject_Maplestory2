#include "stdafx.h"
#include "..\Public\JalJang.h"
#include "GameInstance.h"


CJalJang::CJalJang(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CJalJang::CJalJang(const CJalJang & rhs)
	: CCreature(rhs)
{
}

HRESULT CJalJang::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CJalJang::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3{0.f, 2.5f, 18.f});
	m_pTransformCom->Set_Scaled(3.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CJalJang::SetUp_Components()
{

	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_JalJang"), nullptr);


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CJalJang::Tick(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case Client::CJalJang::STATE_END:
		Tick_Stand(fTimeDelta);
		break;
	case Client::CJalJang::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CJalJang::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CJalJang::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CJalJang::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CJalJang::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_UI, this);
}

HRESULT CJalJang::Render()
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


HRESULT CJalJang::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CJalJang::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}



void CJalJang::Tick_Idle(_float fTimeDelta)
{

}
void CJalJang::Tick_Stand(_float fTimeDelta)
{
	//멤버 변수로 0 초기화 해두고
	// fTimeDelta를 더한다
	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;
		if (m_fTime >= 3.f)
		{
			m_fTime = 0.f;
			SetState(STATE_MOVE, DIR_L);
		}
	}

	if (m_eDir == DIR_R)
	{
		m_fTime += fTimeDelta;
		if (m_fTime >= 3.f)
		{
			m_fTime = 0.f;
			SetState(STATE_MOVE, DIR_R);
		}
	}
}
void CJalJang::Tick_Move(_float fTimeDelta)
{
	if (m_eDir == DIR_R)
	{
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_Right(fTimeDelta);

		if (m_fTime >= 3.f)
		{
			SetState(STATE_END, DIR_L);
			m_fTime = 0.f;
		}
	}

	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_Left(fTimeDelta);

		if (m_fTime >= 3.f)
		{
			SetState(STATE_END, DIR_R);
			m_fTime = 0.f;
		}
	}
}
void CJalJang::Tick_Hit(_float fTimeDelta)
{

}

void CJalJang::Tick_Chase(_float fTimeDelta)
{
}



void CJalJang::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CJalJang::SetAni()
{
	switch (m_eCurState)
	{

	case CJalJang::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_JalJang"), 0.3f, CAnimator::STATE_LOOF);
		break;

	case CJalJang::STATE_HIT:
		break;

	case CJalJang::STATE_CHASE:
		break;
	}
}

void CJalJang::Damaged(CGameObject * pOther)
{

}

CJalJang * CJalJang::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CJalJang*		pInstance = new CJalJang(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CJalJang"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CJalJang::Clone(void* pArg)
{
	CJalJang*		pInstance = new CJalJang(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CJalJang"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CJalJang::Collision(CGameObject * pOther)
{

}




void CJalJang::Free()
{
	__super::Free();

}


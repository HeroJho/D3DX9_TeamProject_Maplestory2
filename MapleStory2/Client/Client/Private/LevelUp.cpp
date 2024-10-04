#include "stdafx.h"
#include "..\Public\LevelUp.h"
#include "GameInstance.h"


CLevelUp::CLevelUp(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CLevelUp::CLevelUp(const CLevelUp & rhs)
	: CCreature(rhs)
{
}

HRESULT CLevelUp::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CLevelUp::Initialize(void * pArg)
{
	__super::Initialize(pArg);
	
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_LevelUp";

	if (nullptr == pArg)
		return E_FAIL;

	CRETUREDESC* pDesc = (CRETUREDESC*)pArg;
	pDesc->vPos.z -= 0.2f;
	pDesc->vPos.y += 0.8f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
	m_pTransformCom->Set_Scaled(7.f);
	m_fTime = 0.f;
	SetState(STATE_IDLE, DIR_L);

	return S_OK;
}

HRESULT CLevelUp::SetUp_Components()
{

	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LevelUp"), nullptr);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevelUp::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);
	_float3 vPos = ((CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos + _float3{ 0.f,0.5f,0.4f });
	Safe_Release(pInstance);
	switch (m_eCurState)
	{
	case Client::CLevelUp::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;

	case Client::CLevelUp::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;

	case Client::CLevelUp::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;

	case Client::CLevelUp::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}
}

void CLevelUp::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		Set_Dead();

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CLevelUp::Render()
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

void CLevelUp::Tick_Idle(_float fTimeDelta)
{

}
void CLevelUp::Tick_Move(_float fTimeDelta)
{
	if (m_eDir == DIR_R)
	{
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_RU(fTimeDelta);

		if (m_fTime >= 5.f)
		{
			SetState(STATE_IDLE, DIR_L);
			m_fTime = 0.f;
		}
	}

	if (m_eDir == DIR_L)
	{
		m_fTime += fTimeDelta;

		m_pTransformCom->Go_LD(fTimeDelta);

		if (m_fTime >= 5.f)
		{
			SetState(STATE_IDLE, DIR_R);
			m_fTime = 0.f;
		}
	}
}
void CLevelUp::Tick_Hit(_float fTimeDelta)
{

}

void CLevelUp::Tick_Chase(_float fTimeDelta)
{
}


HRESULT CLevelUp::Set_RenderState()
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

HRESULT CLevelUp::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}


void CLevelUp::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CLevelUp::SetAni()
{
	switch (m_eCurState)
	{
	case CLevelUp::STATE_IDLE:
		if (m_eDir == DIR_L)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_LevelUp"), 0.07f, CAnimator::STATE_ONCE);
		break;

	}
}

void CLevelUp::Damaged(CGameObject * pOther)
{

}

CLevelUp * CLevelUp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevelUp*		pInstance = new CLevelUp(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHelena"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CLevelUp::Clone(void* pArg)
{
	CLevelUp*		pInstance = new CLevelUp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHelena"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CLevelUp::Collision(CGameObject * pOther)
{

}




void CLevelUp::Free()
{
	__super::Free();

}


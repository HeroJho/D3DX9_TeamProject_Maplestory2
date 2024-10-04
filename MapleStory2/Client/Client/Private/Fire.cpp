#include "stdafx.h"
#include "..\Public\Fire.h"

#include "GameInstance.h"


CFire::CFire(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CFire::CFire(const CFire & rhs)
	: CCreature(rhs)
{
}




HRESULT CFire::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CFire::Initialize(void * pArg)
{
	__super::Initialize(pArg);
	CCreature::CRETUREDESC pFireDesc = *(CCreature::CRETUREDESC*)pArg;
	_float3 vPos = pFireDesc.vPos;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	
 0;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Set_Scaled(5.f);

	SetState(STATE_IDLE, DIR_END);



	return S_OK;
}




HRESULT CFire::SetUp_Components()
{

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RedSkill"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CFire::Tick(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case Client::CFire::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CFire::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CFire::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CFire::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

}
void CFire::LateTick(_float fTimeDelta)
{
	Set_Billboard();

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_NPC, this);

}
HRESULT CFire::Render()
{
	_float4x4		Matrix;
	D3DXMatrixIdentity(&Matrix);

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





void CFire::Tick_Idle(_float fTimeDelta)
{
}
void CFire::Tick_Move(_float fTimeDelta)
{
}
void CFire::Tick_Hit(_float fTimeDelta)
{
}

void CFire::Tick_Chase(_float fTimeDelta)
{
}



void CFire::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CFire::SetAni()
{
	switch (m_eCurState)
	{
	case CFire::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_RedSkill"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case CFire::STATE_MOVE:
	{
	}
	break;
	case CFire::STATE_HIT:
		break;
	case CFire::STATE_CHASE:
		break;
	}
}

void CFire::Damaged(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


	SetState(STATE_HIT, DIR_END);


	Safe_Release(pGameInstance);
}






CFire * CFire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFire*		pInstance = new CFire(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CFire::Clone(void* pArg)
{
	CFire*		pInstance = new CFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CFire::Collision(CGameObject * pOther)
{
}




void CFire::Free()
{
	__super::Free();

}


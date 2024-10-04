#include "stdafx.h"
#include "..\Public\Tomb.h"

#include "GameInstance.h"


CTomb::CTomb(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CTomb::CTomb(const CTomb & rhs)
	: CCreature(rhs)
{
}




HRESULT CTomb::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CTomb::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	CCreature::CRETUREDESC vTomb = *(CCreature::CRETUREDESC*)pArg;

	m_fColRad = 1.f;	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTomb.vPos);
	m_pTransformCom->Set_Scaled(2.f);

	SetState(STATE_IDLE, DIR_END);



	return S_OK;
}




HRESULT CTomb::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.3f, 0.5f, 0.3f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tomb"), nullptr);



	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CTomb::Tick(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case Client::CTomb::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CTomb::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CTomb::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CTomb::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}


}
void CTomb::LateTick(_float fTimeDelta)
{
	Set_Billboard();


	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
}
HRESULT CTomb::Render()
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





void CTomb::Tick_Idle(_float fTimeDelta)
{
}
void CTomb::Tick_Move(_float fTimeDelta)
{
}
void CTomb::Tick_Hit(_float fTimeDelta)
{
}

void CTomb::Tick_Chase(_float fTimeDelta)
{
}



void CTomb::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CTomb::SetAni()
{
	switch (m_eCurState)
	{
	case CTomb::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tomb"), 0.08f, CAnimator::STATE_ONCE);
		break;
	case CTomb::STATE_MOVE:
	{
	}
	break;
	case CTomb::STATE_HIT:
		break;
	case CTomb::STATE_CHASE:
		break;
	}
}

void CTomb::Damaged(CGameObject * pOther)
{
}






CTomb * CTomb::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTomb*		pInstance = new CTomb(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTomb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CTomb::Clone(void* pArg)
{
	CTomb*		pInstance = new CTomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTomb"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CTomb::Collision(CGameObject * pOther)
{
}




void CTomb::Free()
{
	__super::Free();

}


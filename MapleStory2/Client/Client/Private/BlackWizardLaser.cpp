#include "stdafx.h"
#include "..\Public\BlackWizardLaser.h"
#include "GameInstance.h"
#include "UIManager.h"

CBlackWizardLaser::CBlackWizardLaser(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBlackWizardLaser::CBlackWizardLaser(const CBlackWizardLaser & rhs)
	: CCreature(rhs)
{
}

HRESULT CBlackWizardLaser::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBlackWizardLaser::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_DARKMAGE, TEXT("Layer_BlackWizard"), TEXT("Com_Transform"), 0);
	m_bMove = true;
	Safe_Release(pInstance);

	m_pTransformCom->Set_Scaled(_float3{ 3.f,3.f,3.f });
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *((_float3*)pArg));
	m_eCurState = STATE_REGEN;
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackMarbleRegen"), 0.1f, CAnimator::STATE_ONCE);


	return S_OK;
}

HRESULT CBlackWizardLaser::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 1.f, 5.f, 1.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackMarbleRegen"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackMarbleIdle"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackMarbleDie"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_pTransformCom->Set_TransformDesc(TransformDesc);

	return S_OK;
}




void CBlackWizardLaser::Tick(_float fTimeDelta)
{	
	if (m_bMove)
	{
		_float3 vLook = m_pTarget->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook));
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	
}
void CBlackWizardLaser::LateTick(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetAni();
	
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	__super::BoxColCom_Tick(m_pTransformCom);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER_SKILL, this);

}
HRESULT CBlackWizardLaser::Render()
{
	Set_Billboard();
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


	//__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}

CBlackWizardLaser * CBlackWizardLaser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizardLaser*		pInstance = new CBlackWizardLaser(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizardLaser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBlackWizardLaser::Clone(void* pArg)
{
	CBlackWizardLaser*		pInstance = new CBlackWizardLaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizardLaser"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBlackWizardLaser::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Monster")
	{
		if (0 < m_pOther.size())
			return;

		CUIManager* pUIInstance = CUIManager::Get_Instance();
		pUIInstance->Fix_BlackWizardHp(5);
		m_bMove = false;
		m_pOther.push_back(pOther);
		m_eCurState = STATE_DIE;
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackMarbleDie"), 0.1f, CAnimator::STATE_ONCE);
		//m_pTransformCom->Set_Scaled(_float3{ 4.f,6.f,4.f });
	}
	else if (pOther->Get_Tag() != "Tag_Player")
	{
		m_eCurState = STATE_DIE;
		m_bMove = false;
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackMarbleDie"), 0.1f, CAnimator::STATE_ONCE);
		//m_pTransformCom->Set_Scaled(_float3{ 4.f,6.f,4.f });
	}
	else if ("Tag_PushCube" == pOther->Get_Tag())
	{
		if (0 < m_pOther.size())
			return;

		m_bMove = false;
		m_pOther.push_back(pOther);
		m_eCurState = STATE_DIE;
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackMarbleDie"), 0.1f, CAnimator::STATE_ONCE);
	}


}

void CBlackWizardLaser::SetAni()
{
	switch (m_eCurState)
	{
	case Client::CBlackWizardLaser::STATE_REGEN:
		m_eCurState = STATE_IDLE;
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackMarbleIdle"), 0.1f, CAnimator::STATE_LOOF);
		break;
	case Client::CBlackWizardLaser::STATE_IDLE:
		break;
	case Client::CBlackWizardLaser::STATE_DIE:
		Set_Dead();
		break;
	case Client::CBlackWizardLaser::STATE_END:
		break;
	default:
		break;
	}
}

//HRESULT CBlackWizardLaser::Set_RenderState()
//{
//	if (nullptr == m_pGraphic_Device)
//		return E_FAIL;
//
//	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//
//	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//
//	return S_OK;
//
//}
//
//HRESULT CBlackWizardLaser::Reset_RenderState()
//{
//	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//
//	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//	return S_OK;
//}




void CBlackWizardLaser::Free()
{
	__super::Free();
}


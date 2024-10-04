#include "stdafx.h"
#include "..\Public\GASEffect.h"
#include "GameInstance.h"

CGASEffect::CGASEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CGASEffect::CGASEffect(const CGASEffect & rhs)
	: CCreature(rhs)
{
}




HRESULT CGASEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CGASEffect::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_MonsterSkill";

	memcpy(&m_Desc, pArg, sizeof(SUNCROSSHITDESC));

	m_fColRad = 10.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Desc.vPos + _float3(0.f,0.f,0.15f));
	m_pTransformCom->Set_Scaled(3.f);
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_GASEffect"), 0.1f, CAnimator::STATE_ONCE);
	m_fYDistance = 11.f;
	m_iDamage = 3;
	m_fEffectCount = 0.f;

	Set_Billboard();
	_float3 vTest = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vPo = vTest - m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPo);

	Set_Billboard();

	return S_OK;

}



HRESULT CGASEffect::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GASEffect"), nullptr);
	}



	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 5.f, 1.f, 2.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };


	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 4.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CGASEffect::Tick(_float fTimeDelta)
{

}
void CGASEffect::LateTick(_float fTimeDelta)
{
	m_fEffectCount += fTimeDelta;

	__super::BoxColCom_Tick(m_pTransformCom);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER_SKILL, this);
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_fEffectCount >= 1.5f)
		Set_Dead();


}
HRESULT CGASEffect::Render()
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



void CGASEffect::SetState(STATE eState, DIR eDir)
{

}
void CGASEffect::SetAni()
{

}






CGASEffect * CGASEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGASEffect*		pInstance = new CGASEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CGASEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CGASEffect::Clone(void* pArg)
{
	CGASEffect*		pInstance = new CGASEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CGASEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CGASEffect::Collision(CGameObject * pOther)
{

}

HRESULT CGASEffect::Set_RenderState()
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

HRESULT CGASEffect::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pTransformCom->CulRUByLook(m_vLookTemp);
	return S_OK;
}




void CGASEffect::Free()
{
	__super::Free();

}


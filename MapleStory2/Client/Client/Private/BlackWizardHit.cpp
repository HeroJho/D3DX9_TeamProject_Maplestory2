#include "stdafx.h"
#include "..\Public\BlackWizardHit.h"
#include "GameInstance.h"

CBlackWizardHit::CBlackWizardHit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBlackWizardHit::CBlackWizardHit(const CBlackWizardHit & rhs)
	: CCreature(rhs)
{
}

HRESULT CBlackWizardHit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBlackWizardHit::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	/*_float3 vPos;
	memcpy(&vPos, pArg, sizeof(_float3));
	_float3(pArg);*/
	m_pTransformCom->Set_Scaled(4.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)pArg);

	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Hit"), 0.1f, CAnimator::STATE_ONCE);
	Safe_Release(pInstance);

	return S_OK;
}




HRESULT CBlackWizardHit::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Hit"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}




void CBlackWizardHit::Tick(_float fTimeDelta)
{

}
void CBlackWizardHit::LateTick(_float fTimeDelta)
{
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	if (m_pAnimatorCom->Get_AnimCount() == 6)
		Set_Dead();


}
HRESULT CBlackWizardHit::Render()
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

	return S_OK;
}

CBlackWizardHit * CBlackWizardHit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizardHit*		pInstance = new CBlackWizardHit(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizardHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBlackWizardHit::Clone(void* pArg)
{
	CBlackWizardHit*		pInstance = new CBlackWizardHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizardHit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBlackWizardHit::Collision(CGameObject * pOther)
{
}

HRESULT CBlackWizardHit::Set_RenderState()
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

HRESULT CBlackWizardHit::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CBlackWizardHit::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "..\Public\BeastEffect.h"
#include "BeastBegin.h"
#include "GameInstance.h"

CBeastEffect::CBeastEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBeastEffect::CBeastEffect(const CBeastEffect & rhs)
	: CCreature(rhs), m_bCreate(false)
{
}




HRESULT CBeastEffect::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBeastEffect::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	Safe_Release(pInstance);

	m_pTransformCom->Set_Scaled(_float3{ 10.f,8.f,10.f });

	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Beast_Effect"), 0.06f, CAnimator::STATE_ONCE);
	memcpy(&m_Desc, pArg, sizeof(BEASTEFFECTDESC));
	m_eDir = m_Desc.eDir;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f,0.f,-0.1f });
	m_fYDistance = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	return S_OK;
}




HRESULT CBeastEffect::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Beast_Effect"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}




void CBeastEffect::Tick(_float fTimeDelta)
{
	
}
void CBeastEffect::LateTick(_float fTimeDelta)
{

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);

	if (m_pAnimatorCom->Get_AnimCount() == 6 && !m_bCreate)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CBeastBegin::BEASTBEGINDESC BeginDesc;
		BeginDesc.eDir = m_eDir;
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Beast_Begin"), LEVEL_STATIC, TEXT("Layer_Player_Skill"), &BeginDesc);
		Safe_Release(pGameInstance);
		m_bCreate = true;
	}

	if (m_pAnimatorCom->Get_AnimCount() == 6)
	{
		Set_Dead();
	}


}
HRESULT CBeastEffect::Render()
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


void CBeastEffect::SetState(STATE eState, DIR eDir)
{

}


void CBeastEffect::SetAni()
{

}



CBeastEffect * CBeastEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBeastEffect*		pInstance = new CBeastEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBeastEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBeastEffect::Clone(void* pArg)
{
	CBeastEffect*		pInstance = new CBeastEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBeastEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBeastEffect::Collision(CGameObject * pOther)
{

}

HRESULT CBeastEffect::Set_RenderState()
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

HRESULT CBeastEffect::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CBeastEffect::Free()
{
	__super::Free();
}


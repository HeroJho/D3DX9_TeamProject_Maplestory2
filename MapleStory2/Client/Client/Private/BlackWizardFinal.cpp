#include "stdafx.h"
#include "..\Public\BlackWizardFinal.h"
#include "GameInstance.h"
#include "UIManager.h"
#include "ToolManager.h"
#include "CutSceneManager.h"

CBlackWizardFinal::CBlackWizardFinal(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBlackWizardFinal::CBlackWizardFinal(const CBlackWizardFinal & rhs)
	: CCreature(rhs)
{
}

HRESULT CBlackWizardFinal::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBlackWizardFinal::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);
	m_sTag = "Tag_MonsterSkill";
	m_iDamage = 99;
	m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);
	
	CGameInstance::Get_Instance()->PlaySoundW(L"BlackWizardFinishHit.mp3", 5, 1.f);
	Safe_Release(pInstance);

	m_pTransformCom->Set_Scaled(_float3{ 12.f,12.f,12.f });
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f,2.f,0.f });

	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizardFinal"), 0.08f, CAnimator::STATE_ONCE);


	return S_OK;
}




HRESULT CBlackWizardFinal::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 2.5f, 10.f, 2.5f };
	BoxColDesc.vPivot = _float3{ 0.f, -5.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizardFinal"), nullptr);
	}


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}




void CBlackWizardFinal::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f,2.f,0.f });
}
void CBlackWizardFinal::LateTick(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_AnimCount() == 13)
		CCutSceneManager::Get_Instance()->Get_MainCam()->StartShake(0.4f, 0.05f, 15.f, _float3(1.f, 1.f, 1.f));

	if(m_pAnimatorCom->Get_AnimCount() == 20)
		CUIManager::Get_Instance()->Set_PlayerHp(21);

	if (m_pAnimatorCom->Get_AnimCount() == 27)
	{
		CToolManager::Get_Instance()->SetDestLevel(LEVEL_DARKMAGEENTER, _float3{ -1.f, 2.f, 0.f });
	}

	
		
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);

	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER_SKILL, this);


}
HRESULT CBlackWizardFinal::Render()
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

CBlackWizardFinal * CBlackWizardFinal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizardFinal*		pInstance = new CBlackWizardFinal(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizardFinal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBlackWizardFinal::Clone(void* pArg)
{
	CBlackWizardFinal*		pInstance = new CBlackWizardFinal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizardFinal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBlackWizardFinal::Collision(CGameObject * pOther)
{
	/*if (pOther->Get_Tag() == "Tag_Player")
	{
		if (m_pAnimatorCom->Get_AnimCount() == 15)
		{
			if (0 < m_pOther.size())
				return;
		
		
			m_pOther.push_back(pOther);

			pOther->Damaged(this);

		}
	}*/
}

HRESULT CBlackWizardFinal::Set_RenderState()
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

HRESULT CBlackWizardFinal::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CBlackWizardFinal::Free()
{
	__super::Free();
}


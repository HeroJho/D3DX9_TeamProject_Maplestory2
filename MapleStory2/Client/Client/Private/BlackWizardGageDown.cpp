#include "stdafx.h"
#include "..\Public\BlackWizardGageDown.h"
#include "GameInstance.h"
#include "UIManager.h"

CBlackWizardGageDown::CBlackWizardGageDown(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBlackWizardGageDown::CBlackWizardGageDown(const CBlackWizardGageDown & rhs)
	: CCreature(rhs)
{
}

HRESULT CBlackWizardGageDown::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBlackWizardGageDown::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	m_sTag = "Tag_Object";
	Safe_Release(pInstance);

	m_pTransformCom->Set_Scaled(_float3{ 2.f,2.f,2.f });
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *((_float3*)pArg));
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizardGageDown"), 0.1f, CAnimator::STATE_LOOF);


	return S_OK;
}

HRESULT CBlackWizardGageDown::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 1.f, 3.f, 1.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizardGageDown"), nullptr);
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




void CBlackWizardGageDown::Tick(_float fTimeDelta)
{

}
void CBlackWizardGageDown::LateTick(_float fTimeDelta)
{

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);

	__super::BoxColCom_Tick(m_pTransformCom);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER_SKILL, this);

}
HRESULT CBlackWizardGageDown::Render()
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

CBlackWizardGageDown * CBlackWizardGageDown::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizardGageDown*		pInstance = new CBlackWizardGageDown(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizardGageDown"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBlackWizardGageDown::Clone(void* pArg)
{
	CBlackWizardGageDown*		pInstance = new CBlackWizardGageDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizardGageDown"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBlackWizardGageDown::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Player")
	{
		if (0 < m_pOther.size())
			return;
		CUIManager* pUIInstance = CUIManager::Get_Instance();
		pUIInstance->Get_BlackWizardPatternUI()->Set_TextNum(-2);
		pUIInstance->Get_BlackWizardPatternUI()->Shake();
		m_pOther.push_back(pOther);
		CGameInstance::Get_Instance()->PlaySoundW(L"GageDown.mp3", 30, 1.f);
		Set_Dead();		
	}

}

void CBlackWizardGageDown::SetAni()
{
	
}

HRESULT CBlackWizardGageDown::Set_RenderState()
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

HRESULT CBlackWizardGageDown::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CBlackWizardGageDown::Free()
{
	__super::Free();
}


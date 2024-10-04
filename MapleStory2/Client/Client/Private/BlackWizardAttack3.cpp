#include "stdafx.h"
#include "..\Public\BlackWizardAttack3.h"
#include "GameInstance.h"
#include "UIManager.h"
#include "CutSceneManager.h"

CBlackWizardAttack3::CBlackWizardAttack3(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBlackWizardAttack3::CBlackWizardAttack3(const CBlackWizardAttack3 & rhs)
	: CCreature(rhs)
{
}

HRESULT CBlackWizardAttack3::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBlackWizardAttack3::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);
	m_iDamage = 2;
	m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	Safe_Release(pInstance);

	m_pTransformCom->Set_Scaled(_float3{ 4.f,6.f,4.f });
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *((_float3*)pArg) + _float3{ 0.f,2.f,1.f } );
	CGameInstance::Get_Instance()->PlaySoundW(L"BlackWizardAttack3.mp3", 28, 1.f);
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Attack3"), 0.1f, CAnimator::STATE_ONCE);


	return S_OK;
}




HRESULT CBlackWizardAttack3::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 1.5f, 10.f, 1.5f };
	BoxColDesc.vPivot = _float3{ 0.f, -5.f, -1.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Attack3"), nullptr);
	}


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}




void CBlackWizardAttack3::Tick(_float fTimeDelta)
{

}
void CBlackWizardAttack3::LateTick(_float fTimeDelta)
{
	if(m_pAnimatorCom->Get_AnimCount() == 9)
		CCutSceneManager::Get_Instance()->Get_MainCam()->Start_AttackShaking();
	if (m_pAnimatorCom->Get_AnimCount() >= 14)
		Set_Dead();
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);

	__super::BoxColCom_Tick(m_pTransformCom);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER_SKILL, this);


}
HRESULT CBlackWizardAttack3::Render()
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

CBlackWizardAttack3 * CBlackWizardAttack3::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizardAttack3*		pInstance = new CBlackWizardAttack3(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizardAttack3"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBlackWizardAttack3::Clone(void* pArg)
{
	CBlackWizardAttack3*		pInstance = new CBlackWizardAttack3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizardAttack3"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBlackWizardAttack3::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Player")
	{
		if (m_pAnimatorCom->Get_AnimCount() > 9)
		{
			if (0 < m_pOther.size())
				return;

			CUIManager* pUIInstance = CUIManager::Get_Instance();
			pUIInstance->Get_BlackWizardPatternUI()->Set_TextNum(1);
			pUIInstance->Get_BlackWizardPatternUI()->Shake();

			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			_float3 vPos = m_pTarget->Get_State(CTransform::STATE_POSITION);

			pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardHit"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &vPos);
			Safe_Release(pGameInstance);
			m_pOther.push_back(pOther);

			pOther->Damaged(this);

		}
	}
}

HRESULT CBlackWizardAttack3::Set_RenderState()
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

HRESULT CBlackWizardAttack3::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CBlackWizardAttack3::Free()
{
	__super::Free();
}


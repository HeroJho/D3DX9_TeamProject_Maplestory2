#include "stdafx.h"
#include "..\Public\BlackWizardPatternUI.h"
#include "GameInstance.h"
#include "UIManager.h"


CBlackWizardPatternUI::CBlackWizardPatternUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CBlackWizardPatternUI::CBlackWizardPatternUI(const CBlackWizardPatternUI& rhs)
	: CUI(rhs)
{
}


HRESULT CBlackWizardPatternUI::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBlackWizardPatternUI::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 100.f;
	m_UIInfo.fSizeY = 122.f;
	m_UIInfo.fX = 100.f;
	m_UIInfo.fY = 300.f;

	__super::Initialize(pArg);
	m_iTexturenum = 0;
	m_fShakeAcc = 0.f;
	m_bRight = true;
	m_bShake = false;
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizardPatternUI"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	m_bRender = true;
	CUIManager* pUIInstance = CUIManager::Get_Instance();
	pUIInstance->Add_BlackWizardPatternUI(this);

	return S_OK;
}

void CBlackWizardPatternUI::Tick(_float fTimeDelta)
{
	if (m_bShake)
	{
		m_fShakeAcc += 1.f * fTimeDelta;
		if (0.3f < m_fShakeAcc)
		{
			m_bShake = false;
			m_fShakeAcc = 0.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vOriginPos);
			return;
		}
		if (m_bRight)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ 10.f,0.f,0.f });
			m_bRight = false;
		}
		else
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ -10.f,0.f,0.f });
			m_bRight = true;
		}
	}
}

void CBlackWizardPatternUI::LateTick(_float fTimeDelta)
{
	if (m_iTexturenum >= m_pTextureCom->Get_Size())
	{
		CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardScene"), LEVEL_GAMEPLAY, TEXT("Layer_UI"));
		Safe_Release(pGameInstance);
		Set_Dead();
	}
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CBlackWizardPatternUI::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();


	return S_OK;
}

void CBlackWizardPatternUI::Shake()
{
	m_bShake = true;
}

void CBlackWizardPatternUI::Set_TextNum(_int iNum)
{
	 m_iTexturenum += iNum; 
	 if (m_iTexturenum <= 0)
		 m_iTexturenum = 0;

}


CBlackWizardPatternUI* CBlackWizardPatternUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizardPatternUI*		pInstance = new CBlackWizardPatternUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizardPatternUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlackWizardPatternUI::Clone(void * pArg)
{
	CBlackWizardPatternUI*		pInstance = new CBlackWizardPatternUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizardPatternUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackWizardPatternUI::Free()
{
	__super::Free();
}

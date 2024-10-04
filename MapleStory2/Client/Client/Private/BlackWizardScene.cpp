#include "stdafx.h"
#include "..\Public\BlackWizardScene.h"
#include "GameInstance.h"
#include "BlackWizard.h"


CBlackWizardScene::CBlackWizardScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CBlackWizardScene::CBlackWizardScene(const CBlackWizardScene& rhs)
	: CUI(rhs)
{
}


HRESULT CBlackWizardScene::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBlackWizardScene::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = g_iWinSizeX;
	m_UIInfo.fSizeY = g_iWinSizeY;
	m_UIInfo.fX = g_iWinSizeX * 0.5f;
	m_UIInfo.fY = g_iWinSizeY * 0.5f;

	__super::Initialize(pArg);
	m_iTexturenum = 0;
	m_fAniAcc = 0.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizardScene"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	m_bFinal = false;
	m_bRender = true;
	CGameInstance::Get_Instance()->PlaySoundW(L"BlackWizardFinish.wav", 27, 1.f);
	CGameInstance::Get_Instance()->PlaySoundW(L"BlackWizardFinishVoice.wav", 28, 1.f);
	return S_OK;
}

void CBlackWizardScene::Tick(_float fTimeDelta)
{
	m_fAniAcc += 1.f * fTimeDelta;
	if (m_fAniAcc > 0.1f)
	{
		m_fAniAcc = 0.f;
		++m_iTexturenum;
	}
}

void CBlackWizardScene::LateTick(_float fTimeDelta)
{

	if (m_iTexturenum > 12 && !m_bFinal)
	{
		CGameInstance* pInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pInstance);
		((CBlackWizard*)pInstance->Get_ObjectPtr(LEVEL_DARKMAGE, L"Layer_BlackWizard", 0))->Set_Final();
		pInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardFinal"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"));
		Safe_Release(pInstance);
		m_bFinal = true;
	}
	if (m_iTexturenum > m_pTextureCom->Get_Size())
	{		
		Set_Dead();
	}
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CBlackWizardScene::Render()
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


CBlackWizardScene* CBlackWizardScene::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizardScene*		pInstance = new CBlackWizardScene(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizardScene"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlackWizardScene::Clone(void * pArg)
{
	CBlackWizardScene*		pInstance = new CBlackWizardScene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizardScene"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackWizardScene::Free()
{
	__super::Free();
}

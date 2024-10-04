#include "stdafx.h"
#include "..\Public\HpBarBase.h"
#include "GameInstance.h"
#include "UIManager.h"


CHpBarBase::CHpBarBase(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CHpBarBase::CHpBarBase(const CHpBarBase& rhs)
	: CUI(rhs)
{
}


HRESULT CHpBarBase::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CHpBarBase::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	m_UIInfo.fSizeX = 150.f;
	m_UIInfo.fSizeY = 150.f;
	m_UIInfo.fX = g_iWinSizeX*0.5;
	m_UIInfo.fY = 600.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	__super::Initialize(pArg);

	

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBarBase"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CHpBarBase::Tick(_float fTimeDelta)
{
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);
	


	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);
	
}

void CHpBarBase::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CHpBarBase::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();
	return S_OK;
}


CHpBarBase* CHpBarBase::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHpBarBase*		pInstance = new CHpBarBase(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHpBarBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHpBarBase::Clone(void * pArg)
{
	CHpBarBase*		pInstance = new CHpBarBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHpBarBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHpBarBase::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\HpBarIcon.h"
#include "GameInstance.h"
#include "UIManager.h"


CHpBarIcon::CHpBarIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CHpBarIcon::CHpBarIcon(const CHpBarIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CHpBarIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CHpBarIcon::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 150.f;
	m_UIInfo.fSizeY = 150.f;
	m_UIInfo.fX = g_iWinSizeX*0.5;
	m_UIInfo.fY = 600.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	__super::Initialize(pArg);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBarIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CHpBarIcon::Tick(_float fTimeDelta)
{
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);	

	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);

}

void CHpBarIcon::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CHpBarIcon::Render()
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


CHpBarIcon* CHpBarIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHpBarIcon*		pInstance = new CHpBarIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHpBarIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHpBarIcon::Clone(void * pArg)
{
	CHpBarIcon*		pInstance = new CHpBarIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHpBarIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHpBarIcon::Free()
{
	__super::Free();

}

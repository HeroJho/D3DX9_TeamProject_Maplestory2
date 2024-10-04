#include "stdafx.h"
#include "..\Public\MouseItemIcon.h"
#include "GameInstance.h"
#include "MouseManager.h"
#include "Texture.h"


CMouseItemIcon::CMouseItemIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CMouseItemIcon::CMouseItemIcon(const CMouseItemIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CMouseItemIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CMouseItemIcon::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 32.f;
	m_UIInfo.fSizeY = 32.f;

	m_bRender = false;
	m_iTexturenum = 0;
	__super::Initialize(pArg);

	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
	pMouseInstance->Add_pMouseItemIcon(this);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseConsumItemIcon"), TEXT("Com_Texture2"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseStuffItemIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	return S_OK;
}

void CMouseItemIcon::Tick(_float fTimeDelta)
{

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_UIInfo.fX = ptMouse.x;
	m_UIInfo.fY = ptMouse.y;

	Check_Collision(DIMK_LBUTTON);
	Change_Texture();

}

void CMouseItemIcon::LateTick(_float fTimeDelta)
{
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	if (m_iTexturenum > 98)
		m_bRender = false;

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CMouseItemIcon::Render()
{
	switch (m_eType)
	{
	case Client::CInvenManager::TYPE_EQUIP:
		break;
	case Client::CInvenManager::TYPE_CONSUM:
		if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;
		break;
	case Client::CInvenManager::TYPE_STUFF:
		if (FAILED(m_pTextureCom2->Bind_Texture(m_iTexturenum)))
		return E_FAIL;
		break;		
	default:
		break;
	}

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();
	return S_OK;
}

void CMouseItemIcon::Change_Texture()
{

	switch (m_eCollision)
	{
	case Client::CUI::TYPE_ON:
		m_bRender = false;
		m_iTexturenum = 99;
		break;
	case Client::CUI::TYPE_DOWN:
		m_bRender = true;
		break;
	case Client::CUI::TYPE_PRESSING:
		break;
	default:
		break;
	}

}


CMouseItemIcon* CMouseItemIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMouseItemIcon*		pInstance = new CMouseItemIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMouseItemIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMouseItemIcon::Clone(void * pArg)
{
	CMouseItemIcon*		pInstance = new CMouseItemIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMouseItemIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouseItemIcon::Free()
{
	Safe_Release(m_pTextureCom2);
	__super::Free();

}

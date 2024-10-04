#include "stdafx.h"
#include "..\Public\MouseSkillIcon.h"
#include "GameInstance.h"
#include "MouseManager.h"


CMouseSkillIcon::CMouseSkillIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CMouseSkillIcon::CMouseSkillIcon(const CMouseSkillIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CMouseSkillIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CMouseSkillIcon::Initialize(void * pArg)




















{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 32.f;
	m_UIInfo.fSizeY = 32.f;

	m_bRender = false;
	m_iTexturenum = 0;
	__super::Initialize(pArg);

	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
	pMouseInstance->Add_pMouseSkillIcon(this);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseSkillIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	//pSkillInstance->Add_SkillFrameImage(this);

	return S_OK;
}

void CMouseSkillIcon::Tick(_float fTimeDelta)
{

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_UIInfo.fX = ptMouse.x ;
	m_UIInfo.fY = ptMouse.y;

	Check_Collision(DIMK_LBUTTON);
	Change_Texture();

}

void CMouseSkillIcon::LateTick(_float fTimeDelta)
{
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	if (m_iTexturenum > 98)
		m_bRender = false;

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CMouseSkillIcon::Render()
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

void CMouseSkillIcon::Change_Texture()
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


CMouseSkillIcon* CMouseSkillIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMouseSkillIcon*		pInstance = new CMouseSkillIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMouseSkillIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMouseSkillIcon::Clone(void * pArg)
{
	CMouseSkillIcon*		pInstance = new CMouseSkillIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMouseSkillIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouseSkillIcon::Free()
{
	__super::Free();

}

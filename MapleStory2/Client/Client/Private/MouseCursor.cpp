#include "stdafx.h"
#include "..\Public\MouseCursor.h"
#include "GameInstance.h"
#include "MouseManager.h"


CMouseCursor::CMouseCursor(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CMouseCursor::CMouseCursor(const CMouseCursor& rhs)
	: CUI(rhs)
{
}


HRESULT CMouseCursor::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CMouseCursor::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	
	m_UIInfo.fSizeX = 24.f;
	m_UIInfo.fSizeY = 28.f;

	m_iTexturenum = 0;
	__super::Initialize(pArg);

	//CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseCursor"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	//pSkillInstance->Add_SkillFrameImage(this);

	return S_OK;
}

void CMouseCursor::Tick(_float fTimeDelta)
{

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_UIInfo.fX = ptMouse.x+10.f;
	m_UIInfo.fY = ptMouse.y+14.f;	

	Check_Collision(DIMK_LBUTTON);
	Change_Texture();

}

void CMouseCursor::LateTick(_float fTimeDelta)
{
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);


}

HRESULT CMouseCursor::Render()
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

void CMouseCursor::Change_Texture()
{

	switch (m_eCollision)
	{
	case Client::CUI::TYPE_ON:
		m_iTexturenum = 0;
		break;
	case Client::CUI::TYPE_DOWN:
		m_iTexturenum = 1;
		break;
	case Client::CUI::TYPE_PRESSING:
		m_iTexturenum = 2;
		break;
	default:
		break;
	}

}

HRESULT CMouseCursor::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 170);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	return S_OK;
}

HRESULT CMouseCursor::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CMouseCursor* CMouseCursor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMouseCursor*		pInstance = new CMouseCursor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMouseCursor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMouseCursor::Clone(void * pArg)
{
	CMouseCursor*		pInstance = new CMouseCursor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMouseCursor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouseCursor::Free()
{
	__super::Free();

}

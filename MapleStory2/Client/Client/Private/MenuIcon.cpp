#include "stdafx.h"
#include "..\Public\MenuIcon.h"
#include "GameInstance.h"
#include "UIManager.h"

CMenuIcon::CMenuIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CMenuIcon::CMenuIcon(const CMenuIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CMenuIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CMenuIcon::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 36.f;
	m_UIInfo.fSizeY = 53.f;
	m_UIInfo.fX = 1240.f;
	m_UIInfo.fY = 650.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	__super::Initialize(pArg);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CMenuIcon::Tick(_float fTimeDelta)
{
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);



	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);

}

void CMenuIcon::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CMenuIcon::Render()
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


CMenuIcon* CMenuIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMenuIcon*		pInstance = new CMenuIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMenuIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMenuIcon::Clone(void * pArg)
{
	CMenuIcon*		pInstance = new CMenuIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMenuIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMenuIcon::Free()
{
	__super::Free();

}

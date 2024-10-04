#include "stdafx.h"
#include "..\Public\ExpBase.h"
#include "GameInstance.h"
#include "UIManager.h"


CExpBase::CExpBase(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CExpBase::CExpBase(const CExpBase& rhs)
	: CUI(rhs)
{
}


HRESULT CExpBase::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CExpBase::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 1280.f;
	m_UIInfo.fSizeY = 17.f;
	m_UIInfo.fX = g_iWinSizeX*0.5;
	m_UIInfo.fY = g_iWinSizeY - m_UIInfo.fSizeY*0.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	__super::Initialize(pArg);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ExpBase"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CExpBase::Tick(_float fTimeDelta)
{
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);

	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);

}

void CExpBase::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CExpBase::Render()
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


CExpBase* CExpBase::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExpBase*		pInstance = new CExpBase(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CExpBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CExpBase::Clone(void * pArg)
{
	CExpBase*		pInstance = new CExpBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CExpBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExpBase::Free()
{
	__super::Free();

}

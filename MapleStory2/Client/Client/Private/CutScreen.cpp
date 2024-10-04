#include "stdafx.h"
#include "..\Public\CutScreen.h"

#include "GameInstance.h"


CCutScreen::CCutScreen(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CCutScreen::CCutScreen(const CCutScreen& rhs)
	: CUI(rhs)
{
}


HRESULT CCutScreen::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCutScreen::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 1280;
	m_UIInfo.fSizeY = 720;
	m_UIInfo.fX = g_iWinSizeX*0.5f;
	m_UIInfo.fY = g_iWinSizeY*0.5f;
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_iTexturenum = 0;

	__super::Initialize(pArg);


	return S_OK;
}

void CCutScreen::OnCutScreen()
{
	m_bCutScreen = true;
	m_iCount = 0;
	m_fTimeAcc = 0.f;
}
void CCutScreen::OffCutScreen()
{
	m_bCutScreen = false;
	m_iCount = 9;
	m_fTimeAcc = 0.f;
}


void CCutScreen::Tick(_float fTimeDelta)
{

	if (m_bCutScreen)
	{

		m_fTimeAcc += fTimeDelta;

		if (0.05f < m_fTimeAcc)
		{
			++m_iCount;
			m_fTimeAcc = 0.f;
			if (9 < m_iCount)
				m_iCount = 9;
		}

	}
	else
	{

		m_fTimeAcc += fTimeDelta;

		if (0.05f < m_fTimeAcc)
		{
			--m_iCount;
			m_fTimeAcc = 0.f;
			if (0 > m_iCount)
				m_iCount = 0;
		}

	}



}

void CCutScreen::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCutScreen::Render()
{
	if (!m_bRender)
		return S_OK;



	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_Texture(m_iCount)))
		return E_FAIL;

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();
	return S_OK;
}



HRESULT CCutScreen::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CutScreen"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}


CCutScreen* CCutScreen::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCutScreen*		pInstance = new CCutScreen(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCutScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCutScreen::Clone(void * pArg)
{
	CCutScreen*		pInstance = new CCutScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCutScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCutScreen::Free()
{
	__super::Free();

}

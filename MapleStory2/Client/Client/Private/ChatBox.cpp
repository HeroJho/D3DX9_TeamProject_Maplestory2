#include "stdafx.h"
#include "..\Public\ChatBox.h"

#include "GameInstance.h"


CChatBox::CChatBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CChatBox::CChatBox(const CChatBox& rhs)
	: CUI(rhs)
{
}





HRESULT CChatBox::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CChatBox::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = g_iWinSizeX * 0.8f;
	m_UIInfo.fSizeY = g_iWinSizeY * 0.6f;
	m_UIInfo.fX = g_iWinSizeX * 0.5f;
	m_UIInfo.fY = g_iWinSizeY - g_iWinSizeY * 0.3f;
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_iTexturenum = 0;

	__super::Initialize(pArg);



	m_bRender = true;




	D3DXCreateFont(m_pGraphic_Device, 21, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"), &m_pFont);
	D3DXCreateFont(m_pGraphic_Device, 25, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Tip"), &m_pTipFont);


	return S_OK;
}





void CChatBox::Tick(_float fTimeDelta)
{

	if (m_bOn)
		On_Chat(fTimeDelta);
	else
		Off_Chat(fTimeDelta);

}

void CChatBox::LateTick(_float fTimeDelta)
{
	

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CChatBox::Render()
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








void CChatBox::Start_Chat()
{
	m_bOn = true;
	m_fTimeAcc = 0.f;
	m_iCount = 0;

	m_bRender = true;
}

void CChatBox::End_Chat()
{
	m_bOn = false;
	m_fTimeAcc = 0.f;
	m_iCount = 17;
}

void CChatBox::On_Chat(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.01f < m_fTimeAcc)
	{
		++m_iCount;
		if (17 < m_iCount)
		{
			m_iCount = 17;
		}

		m_fTimeAcc = 0.f;
	}
}

void CChatBox::Off_Chat(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.01f < m_fTimeAcc)
	{
		--m_iCount;
		if (0 > m_iCount)
		{
			m_iCount = 0;
			m_bRender = false;
		}

		m_fTimeAcc = 0.f;
	}
}







HRESULT CChatBox::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}







CChatBox* CChatBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChatBox*		pInstance = new CChatBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChatBox::Clone(void * pArg)
{
	CChatBox*		pInstance = new CChatBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChatBox::Free()
{
	__super::Free();

}

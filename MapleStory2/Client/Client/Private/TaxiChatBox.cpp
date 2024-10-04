#include "stdafx.h"
#include "..\Public\TaxiChatBox.h"

#include "GameInstance.h"
#include "TaxiButton.h"


CTaxiChatBox::CTaxiChatBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CTaxiChatBox::CTaxiChatBox(const CTaxiChatBox& rhs)
	: CUI(rhs)
{
}





HRESULT CTaxiChatBox::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CTaxiChatBox::Initialize(void * pArg)
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


	D3DXCreateFont(m_pGraphic_Device, 29, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"), &m_pFont);
	D3DXCreateFont(m_pGraphic_Device, 32, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Name"), &m_pNameFont);



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);




	CTaxiButton::TAXIBUTTONDESC ButtonDesc;
	ZeroMemory(&ButtonDesc, sizeof(CTaxiButton::TAXIBUTTONDESC));
	ButtonDesc.eLevel = LEVEL_ELENYAENTER;
	ButtonDesc.vPos = _float2{ 250.f, 590.f };
	CGameObject* pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TaxiButton"), &pObj, &ButtonDesc)))
		return E_FAIL;
	m_Buttons.push_back((CTaxiButton*)pObj);

	ButtonDesc.eLevel = LEVEL_ELENYA;
	ButtonDesc.vPos = _float2{ 250.f, 640.f };
	pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TaxiButton"), &pObj, &ButtonDesc)))
		return E_FAIL;
	m_Buttons.push_back((CTaxiButton*)pObj);





	Safe_Release(pGameInstance);

	return S_OK;
}





void CTaxiChatBox::Tick(_float fTimeDelta)
{

	if (m_bOn)
		On_Chat(fTimeDelta);
	else
		Off_Chat(fTimeDelta);


	for (auto& pButton : m_Buttons)
		pButton->Tick(fTimeDelta);;
}

void CTaxiChatBox::LateTick(_float fTimeDelta)
{
	for (auto& pButton : m_Buttons)
		pButton->LateTick(fTimeDelta);;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CTaxiChatBox::Render()
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


	RECT rc;

	TCHAR cChat[128];




	if (m_bOn && 17 == m_iCount)
	{
		wsprintf(cChat, TEXT("모범 택시"));
		SetRect(&rc, 260.f - 40.f, 500.f, 0, 0);
		m_pNameFont->DrawText(NULL, cChat,
			-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));


		wsprintf(cChat, TEXT("용사님! 어디로 모실까요?"));
		SetRect(&rc, 260.f - 40.f, 535.f, 0, 0);
		m_pFont->DrawText(NULL, cChat,
			-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		for (auto& pButton : m_Buttons)
			pButton->Render();
	}

	return S_OK;
}








void CTaxiChatBox::Start_Chat()
{
	m_bOn = true;
	m_fTimeAcc = 0.f;
	m_iCount = 0;

	m_bRender = true;
}

void CTaxiChatBox::End_Chat()
{
	m_bOn = false;
	m_fTimeAcc = 0.f;
	m_iCount = 17;
}

void CTaxiChatBox::On_Chat(_float fTimeDelta)
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

void CTaxiChatBox::Off_Chat(_float fTimeDelta)
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







HRESULT CTaxiChatBox::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}







CTaxiChatBox* CTaxiChatBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTaxiChatBox*		pInstance = new CTaxiChatBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTaxiChatBox::Clone(void * pArg)
{
	CTaxiChatBox*		pInstance = new CTaxiChatBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTaxiChatBox::Free()
{
	__super::Free();

	for (auto& pButton : m_Buttons)
		Safe_Release(pButton);

}

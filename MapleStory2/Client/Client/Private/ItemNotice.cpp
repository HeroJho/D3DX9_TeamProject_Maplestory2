#include "stdafx.h"
#include "..\Public\ItemNotice.h"
#include "GameInstance.h"
#include "UIManager.h"



CItemNotice::CItemNotice(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CItemNotice::CItemNotice(const CItemNotice& rhs)
	: CUI(rhs)
{
}


HRESULT CItemNotice::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CItemNotice::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 173.f;
	m_UIInfo.fSizeY = 87.f;
	m_iTexturenum = 99;
	
	__super::Initialize(pArg);
	m_bRender = false;
	
	
	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemNotice"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 12, 0, FW_SEMIBOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"고딕체", &m_TitleFont);

	D3DXCreateFont(m_pGraphic_Device, 10, 0, FW_THIN, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"고딕체", &m_TextFont);

	CUIManager* pUIInstance = CUIManager::Get_Instance();
	pUIInstance->Add_ItemNotice(this);

	return S_OK;
}

void CItemNotice::Tick(_float fTimeDelta)
{
}

void CItemNotice::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		m_UIInfo.fX = ptMouse.x + 100.f;
		m_UIInfo.fY = ptMouse.y + 50.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CItemNotice::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	RenderNotice();
	

	Reset_RenderState();
	return S_OK;
}

void CItemNotice::RenderNotice()
{
	switch (m_iTexturenum)
	{
	case 0:
		RedPortionNotice();
		break;
	case 1:
		BluePortionNotice();
		break;
	case 2:
		TeleportNotice();		
		break;
	case 3:
		BlueSnailNotice();
		break;
	case 4:
		RibbonNotice();
		break;
	case 5:
		SlimeNotice();
		break;
	case 6:
		GreenmushroomNotice();
		break;
	case 7:
		WoodNotice();
		break;
	case 8:
		RedSnailNotice();
		break;
		

	default:
		m_cTitle = "";
		m_cText = "";
		break;
	}
}


void CItemNotice::RedPortionNotice()
{
	m_cTitle = "빨간 포션";
	m_cText = "HP를 약 50 회복시킨다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 25.f, m_UIInfo.fY-34.f , 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 33.f, m_UIInfo.fY-2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::BlueSnailNotice()
{
	m_cTitle = "파란 달팽이의 껍질";
	m_cText = "파란 달팽이의 껍질을 벗긴 것이다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 25.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 29.f, m_UIInfo.fY - 2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::RedSnailNotice()
{
	m_cTitle = "빨간 달팽이의 껍질";
	m_cText = "빨간 달팽이의 껍질을 벗긴 것이다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 25.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 29.f, m_UIInfo.fY - 2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::WoodNotice()
{
	m_cTitle = "장작";
	m_cText = "엑스텀프에서 떼어낸 \n고품질 장작이다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 20.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 32.f, m_UIInfo.fY - 2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::SlimeNotice()
{
	m_cTitle = "물컹물컹한 액체";
	m_cText = "점성이 높아 끈적끈적한 \n액체이다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 43.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 33.f, m_UIInfo.fY - 2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::RibbonNotice()
{
	m_cTitle = "영웅의 리본";
	m_cText = "리본돼지의 리본을 끊은 것이다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 25.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 29.f, m_UIInfo.fY - 2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::GreenmushroomNotice()
{
	m_cTitle = "초록버섯의 갓";
	m_cText = "버섯의 갓을 자른 것이다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 25.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 29.f, m_UIInfo.fY - 2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::BluePortionNotice()
{
	m_cTitle = "파란 포션";
	m_cText = "MP를 약 100 회복시킨다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 25.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 33.f, m_UIInfo.fY - 2, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}

void CItemNotice::TeleportNotice()
{
	m_cTitle = "마을 귀환 주문서";
	m_cText = "현재 위치에서 가장 가까\n운 마을로 귀환할 수 있\n는 주문서이다.";

	RECT Title;
	SetRect(&Title, m_UIInfo.fX - 50.f, m_UIInfo.fY - 34.f, 0, 0);
	m_TitleFont->DrawTextA(NULL, m_cTitle, -1, &Title, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));

	RECT Text;
	SetRect(&Text, m_UIInfo.fX - 33.f, m_UIInfo.fY - 9.f, 0, 0);
	m_TextFont->DrawTextA(NULL, m_cText, -1, &Text, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.0f));
}



CItemNotice* CItemNotice::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItemNotice*		pInstance = new CItemNotice(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CItemNotice"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemNotice::Clone(void * pArg)
{
	CItemNotice*		pInstance = new CItemNotice(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CItemNotice"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemNotice::Free()
{
	__super::Free();

}

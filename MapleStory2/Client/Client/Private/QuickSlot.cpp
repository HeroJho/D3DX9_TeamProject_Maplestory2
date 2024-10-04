#include "stdafx.h"
#include "..\Public\QuickSlot.h"
#include "GameInstance.h"
#include "UIManager.h"

CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CQuickSlot::CQuickSlot(const CQuickSlot& rhs)
	: CUI(rhs)
{
}


HRESULT CQuickSlot::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CQuickSlot::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));

	m_iIndexNum = m_UIInfo.iNum;

	__super::Initialize(pArg);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuickSlot"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_KeyFont);

	return S_OK;
}

void CQuickSlot::Tick(_float fTimeDelta)
{
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);



	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);

}

void CQuickSlot::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CQuickSlot::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	Draw_KeyText();

	return S_OK;
}

void CQuickSlot::Draw_KeyText()
{
	switch (m_iIndexNum)
	{
	case 0:
		RECT KeyQ;
		SetRect(&KeyQ, m_UIInfo.fX-20.f, m_UIInfo.fY-20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"Q", -1, &KeyQ, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 1:
		RECT KeyA;
		SetRect(&KeyA, m_UIInfo.fX-20.f, m_UIInfo.fY-20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"A", -1, &KeyA, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 2:
		RECT KeyW;
		SetRect(&KeyW, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"W", -1, &KeyW, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 3:
		RECT KeyS;
		SetRect(&KeyS, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"S", -1, &KeyS, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 4:
		RECT KeyE;
		SetRect(&KeyE, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"E", -1, &KeyE, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 5:
		RECT KeyD;
		SetRect(&KeyD, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"D", -1, &KeyD, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 6:
		RECT KeyR;
		SetRect(&KeyR, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"R", -1, &KeyR, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 7:
		RECT KeyF;
		SetRect(&KeyF, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"F", -1, &KeyF, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 8:
		RECT Key1;
		SetRect(&Key1, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"1", -1, &Key1, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 9:
		RECT Key5;
		SetRect(&Key5, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"5", -1, &Key5, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 10:
		RECT Key2;
		SetRect(&Key2, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"2", -1, &Key2, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 11:
		RECT Key6;
		SetRect(&Key6, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"6", -1, &Key6, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 12:
		RECT Key3;
		SetRect(&Key3, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"3", -1, &Key3, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 13:
		RECT Key7;
		SetRect(&Key7, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"7", -1, &Key7, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 14:
		RECT Key4;
		SetRect(&Key4, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"4", -1, &Key4, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	case 15:
		RECT Key8;
		SetRect(&Key8, m_UIInfo.fX - 20.f, m_UIInfo.fY - 20.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"8", -1, &Key8, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;

	case 16:
		RECT KeyNum4;
		SetRect(&KeyNum4, m_UIInfo.fX - 18.f, m_UIInfo.fY - 18.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"Num4", -1, &KeyNum4, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;

	case 17:
		RECT KeyNum1;
		SetRect(&KeyNum1, m_UIInfo.fX - 18.f, m_UIInfo.fY - 18.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"Num1", -1, &KeyNum1, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;

	case 18:
		RECT KeyNum5;
		SetRect(&KeyNum5, m_UIInfo.fX - 18.f, m_UIInfo.fY - 18.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"Num5", -1, &KeyNum5, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;

	case 19:
		RECT KeyNum2;
		SetRect(&KeyNum2, m_UIInfo.fX - 18.f, m_UIInfo.fY - 18.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"Num2", -1, &KeyNum2, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;

	case 20:
		RECT KeyNum6;
		SetRect(&KeyNum6, m_UIInfo.fX - 18.f, m_UIInfo.fY - 18.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"Num6", -1, &KeyNum6, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;

	case 21:
		RECT KeyNum3;
		SetRect(&KeyNum3, m_UIInfo.fX - 18.f, m_UIInfo.fY - 18.f, 0, 0);
		m_KeyFont->DrawText(NULL, L"Num3", -1, &KeyNum3, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));
		break;
	default:
		break;
	}
}


CQuickSlot* CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQuickSlot*		pInstance = new CQuickSlot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CQuickSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuickSlot::Clone(void * pArg)
{
	CQuickSlot*		pInstance = new CQuickSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CQuickSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuickSlot::Free()
{
	__super::Free();

}

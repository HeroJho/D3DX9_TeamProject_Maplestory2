#include "stdafx.h"
#include "..\Public\QuickSlotItem.h"
#include "GameInstance.h"
#include "QuickSlotManager.h"
#include "MouseManager.h"
#include "ItemInfo.h"
#include "UIManager.h"




CQuickSlotItem::CQuickSlotItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CQuickSlotItem::CQuickSlotItem(const CQuickSlotItem& rhs)
	: CUI(rhs)
{
}


HRESULT CQuickSlotItem::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CQuickSlotItem::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));
	m_iIndexNum = m_UIInfo.iNum;


	__super::Initialize(pArg);
	m_iTexturenum = 99;
	m_eType = CInvenManager::TYPE_END;
	m_pItemInfoTag = nullptr;
	m_pItemNotice = nullptr;
	m_bRender = false;
	CQuickSlotManager* pQuickSlotInstance = CQuickSlotManager::Get_Instance();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseConsumItemIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	pQuickSlotInstance->Add_QuickSlotItem(this);

	D3DXCreateFont(m_pGraphic_Device, 13, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_NoticeFont);

	D3DXCreateFont(m_pGraphic_Device, 11, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_NowNumFont);


	return S_OK;
}

void CQuickSlotItem::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CQuickSlotManager* pQuickSlotInstance = CQuickSlotManager::Get_Instance();
	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();

	Check_Collision(DIMK_LBUTTON);

	Change_Texture();

	if (pGameInstance->Mouse_Up(DIMK_LBUTTON) && m_eCollision == TYPE_NO && pQuickSlotInstance->Check_Delete() && pMouseInstance->Get_PickType() == CMouseManager::TYPE_QUICK)
	{
		pQuickSlotInstance->Clear_Data(pMouseInstance->Get_Indexnum());
	}

	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);

	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);
	Safe_Release(pGameInstance);
}

void CQuickSlotItem::LateTick(_float fTimeDelta)
{
	if (m_pItemInfoTag != nullptr)
	{
		CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
		if (pInvenInstance->Get_ItemInfo(m_pItemInfoTag, m_eType)->Get_NowNum() == 0)
			Clear_Data();
	}
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CQuickSlotItem::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;

	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();


	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	wchar_t NowNum[10];
	_itow_s(pInvenInstance->Get_ItemInfo(m_pItemInfoTag, m_eType)->Get_NowNum(), NowNum, 10);

	RECT ItemNum;
	SetRect(&ItemNum, m_UIInfo.fX - 15.f, m_UIInfo.fY + 5.f, 0, 0);
	m_NoticeFont->DrawText(NULL, NowNum, -1, &ItemNum, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));


	return S_OK;
}

void CQuickSlotItem::Change_Texture()
{
	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
	CQuickSlotManager* pQuickSlotInstance = CQuickSlotManager::Get_Instance();

	switch (m_eCollision)
	{
	case Client::CUI::TYPE_NO:
		break;
	case Client::CUI::TYPE_ON:

		break;
	case Client::CUI::TYPE_DOWN:
		CGameInstance::Get_Instance()->PlaySoundW(L"DragStart.mp3", 25, 1.f);
		pMouseInstance->Set_ItemIconIndex(CMouseManager::TYPE_QUICK, m_pItemInfoTag, m_eType, m_iTexturenum, m_pItemNotice, m_iIndexNum);
		break;
	case Client::CUI::TYPE_UP:
		if (!pQuickSlotInstance->Check_ItemTexture(pMouseInstance->Get_ItemIconTextnum()))
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragEnd.mp3", 25, 1.f);
			m_pItemInfoTag = pMouseInstance->Get_ItemInfoTag();
			m_eType = pMouseInstance->Get_Type();
			m_iTexturenum = pMouseInstance->Get_ItemIconTextnum();
			m_pItemNotice = pMouseInstance->Get_ItemNotice();

		}
		else
		{
			if (pMouseInstance->Get_PickType() == CMouseManager::TYPE_QUICK)
			{
				CGameInstance::Get_Instance()->PlaySoundW(L"DragEnd.mp3", 25, 1.f);
				pQuickSlotInstance->Change_ItemSlot(pMouseInstance->Get_Indexnum(), this);
				m_pItemInfoTag = pMouseInstance->Get_ItemInfoTag();
				m_eType = pMouseInstance->Get_Type();
				m_iTexturenum = pMouseInstance->Get_ItemIconTextnum();
				m_pItemNotice = pMouseInstance->Get_ItemNotice();
			}
		}
		m_bRender = true;
		break;
	case Client::CUI::TYPE_PRESSING:
		break;
	case Client::CUI::TYPE_END:
		break;
	default:
		break;
	}

}

void CQuickSlotItem::Clear_Data()
{
	m_iTexturenum = 99;
	m_pItemInfoTag = nullptr;
	m_pItemNotice = nullptr;
	m_eType = CInvenManager::TYPE_END;
}





CQuickSlotItem* CQuickSlotItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQuickSlotItem*		pInstance = new CQuickSlotItem(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CQuickSlotItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuickSlotItem::Clone(void * pArg)
{
	CQuickSlotItem*		pInstance = new CQuickSlotItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CQuickSlotItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuickSlotItem::Free()
{
	__super::Free();

}

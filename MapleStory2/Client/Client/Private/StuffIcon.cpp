#include "stdafx.h"
#include "..\Public\StuffIcon.h"
#include "GameInstance.h"
#include "InvenManager.h"
#include "MouseManager.h"
#include "UIManager.h"


CStuffIcon::CStuffIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CStuffIcon::CStuffIcon(const CStuffIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CStuffIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CStuffIcon::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));

	__super::Initialize(pArg);
	m_bRender = false;
	m_iTexturenum = m_UIInfo.iTextNum;
	m_iIndexNum = m_UIInfo.iNum;
	m_pTag = m_UIInfo.pTag;
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();

	m_pItemInfo = pInvenInstance->Get_ItemInfo(m_pTag, CInvenManager::TYPE_STUFF);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StuffIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 11, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_NumFont);

	pInvenInstance->Add_InvenIcon(CInvenManager::TYPE_STUFF, this);

	return S_OK;
}

void CStuffIcon::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	if (pInstance->Key_Down(DIK_I))
	{
		m_bRender = !m_bRender;
	}
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	if (pInvenInstance->Get_InvenType() == CInvenManager::TYPE_STUFF)
	{
		if (pInstance->Mouse_Up(DIMK_RBUTTON) && m_eCollision == TYPE_ON)
		{
			if (m_pItemInfo->Get_NowNum() != 0)
				m_pItemInfo->Set_NowNum(-1);
			if (m_pItemInfo->Get_NowNum() == 0)
			{

				m_pItemInfo = pInvenInstance->Find_ItemInfo(TEXT("DefaultInfo"), CInvenManager::TYPE_STUFF);
				m_pTag = m_pItemInfo->Get_ItemName();
			}
		}
	}
	Check_Collision(DIMK_LBUTTON);

	Safe_Release(pInstance);
}

void CStuffIcon::LateTick(_float fTimeDelta)
{
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	Change_Texture();
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	if (m_bRender&& pInvenInstance->Get_InvenType() == CInvenManager::TYPE_STUFF)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);


}

HRESULT CStuffIcon::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	wchar_t NowNum[10];
	_itow_s(m_pItemInfo->Get_NowNum(), NowNum, 10);

	RECT ItemNum;
	SetRect(&ItemNum, m_UIInfo.fX - 10.f, m_UIInfo.fY + 3.f, 0, 0);
	m_NumFont->DrawText(NULL, NowNum, -1, &ItemNum, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 255.0f, 1.0f));

	if (m_eCollision == TYPE_ON)
	{
		Set_Notice();
	}

	if (m_eCollision == TYPE_NO)
	{
		CUIManager* pUIInstance = CUIManager::Get_Instance();
		if (pUIInstance->Check_Change(m_iTexturenum+3))
			pUIInstance->Set_ItemNoticeTextNum(99, false);
	}

	return S_OK;
}

HRESULT CStuffIcon::Set_ItemInfo(const _tchar * pTag)
{
	if (pTag == nullptr)
		return E_FAIL;
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	m_pItemInfo = pInvenInstance->Get_ItemInfo(pTag, CInvenManager::TYPE_STUFF);
	m_pTag = m_pItemInfo->Get_ItemName();
	
	return S_OK;
}

void CStuffIcon::Set_NowNum(_uint iNum)
{
	m_pItemInfo->Set_NowNum(iNum);
}

void CStuffIcon::Change_Texture()
{
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	if (pInvenInstance->Get_InvenType() == CInvenManager::TYPE_STUFF)
	{
		if (pInvenInstance->Get_ItemInfo(m_pTag, CInvenManager::TYPE_STUFF)->Get_NowNum() < 1)
		{
			m_iTexturenum = 99;
		}
		else
			m_iTexturenum = m_pItemInfo->Get_TextNum();


		CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
		if (m_eCollision == TYPE_DOWN)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragStart.mp3", 25, 1.f);
			pMouseInstance->Set_ItemIconIndex(CMouseManager::TYPE_ITEM, m_pTag, CInvenManager::TYPE_STUFF, m_iTexturenum, m_pItemInfo->Get_ItemNotice(), m_UIInfo.iNum);
		}

		if (m_eCollision == TYPE_UP)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragEnd.mp3", 25, 1.f);
			CItemInfo* pTemp = pMouseInstance->Get_ItemInfo();
			pInvenInstance->Change_Info(m_pTag, pMouseInstance->Get_Indexnum(), CInvenManager::TYPE_STUFF);
			m_pItemInfo = pTemp;
			m_pTag = pTemp->Get_ItemName();

		}
	}
}

void CStuffIcon::Set_Notice()
{
	CUIManager* pUIInstance = CUIManager::Get_Instance();
	switch (m_iTexturenum)
	{
	case 0:
		pUIInstance->Set_ItemNoticeTextNum(3, true);
		break;
	case 1:
		pUIInstance->Set_ItemNoticeTextNum(4, true);
		break;
	case 2:
		pUIInstance->Set_ItemNoticeTextNum(5, true);
		break;
	case 3:
		pUIInstance->Set_ItemNoticeTextNum(6, true);
		break;
	case 4:
		pUIInstance->Set_ItemNoticeTextNum(7, true);
		break;
	case 5:
		pUIInstance->Set_ItemNoticeTextNum(8, true);
		break;
	default:
		break;
	}
}

CStuffIcon* CStuffIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStuffIcon*		pInstance = new CStuffIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CStuffIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStuffIcon::Clone(void * pArg)
{
	CStuffIcon*		pInstance = new CStuffIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CStuffIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStuffIcon::Free()
{
	__super::Free();
}

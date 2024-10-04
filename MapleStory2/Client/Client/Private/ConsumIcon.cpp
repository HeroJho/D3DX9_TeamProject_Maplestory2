#include "stdafx.h"
#include "..\Public\ConsumIcon.h"
#include "GameInstance.h"
#include "InvenManager.h"
#include "MouseManager.h"
#include "UIManager.h"
#include "ToolManager.h"




CConsumIcon::CConsumIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CConsumIcon::CConsumIcon(const CConsumIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CConsumIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CConsumIcon::Initialize(void * pArg)
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

	m_pItemInfo = pInvenInstance->Get_ItemInfo(m_pTag, CInvenManager::TYPE_CONSUM);


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ConsumIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 11, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_NumFont);

	pInvenInstance->Add_InvenIcon(CInvenManager::TYPE_CONSUM, this);

	return S_OK;
}

void CConsumIcon::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	if (pInstance->Key_Down(DIK_I))
	{
		m_bRender = !m_bRender;
	}
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	if (pInstance->Mouse_Up(DIMK_RBUTTON) && m_eCollision == TYPE_ON && pInvenInstance->Get_InvenType() == CInvenManager::TYPE_CONSUM)
	{
		if (m_pItemInfo->Get_NowNum() != 0)
		{
			m_pItemInfo->Set_NowNum(-1);
			Use_Item();
		}
		if (m_pItemInfo->Get_NowNum() == 0)
		{
			
			m_pItemInfo = pInvenInstance->Find_ItemInfo(TEXT("DefaultInfo"), CInvenManager::TYPE_CONSUM);
			m_pTag = m_pItemInfo->Get_ItemName();
		}
	}
	Check_Collision(DIMK_LBUTTON);

	if (m_pItemInfo->Get_TextNum() == 0)
	{
		if (pInstance->Key_Down(DIK_NUMPAD1))
		{
			m_pItemInfo->Set_NowNum(-1);
			Use_Item();
		}
		
	}
	else if (m_pItemInfo->Get_TextNum() == 1)
	{
		if (pInstance->Key_Down(DIK_NUMPAD2))
		{
			m_pItemInfo->Set_NowNum(-1);
			Use_Item();
		}

	}

	else if (m_pItemInfo->Get_TextNum() == 2)
	{
		if (pInstance->Key_Down(DIK_NUMPAD3))
		{
			m_pItemInfo->Set_NowNum(-1);
			pInstance->PlaySoundW(L"Portal.mp3", 20, 1.f);
			Use_Item();
		}

	}

	

	Safe_Release(pInstance);
}

void CConsumIcon::LateTick(_float fTimeDelta)
{
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	Change_Texture();
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	if (m_bRender&& pInvenInstance->Get_InvenType() == CInvenManager::TYPE_CONSUM)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);



	if (m_bSceneChange)
	{
		m_fTimeAcc += fTimeDelta;

		if (5.f < m_fTimeAcc)
		{
			CToolManager::Get_Instance()->SetDestLevel(LEVEL_GAMEPLAY, _float3{ -9.f, 4.f, -3.f });
			m_bSceneChange = false;
		}
	}


}

HRESULT CConsumIcon::Render()
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
	SetRect(&ItemNum, m_UIInfo.fX-12.f, m_UIInfo.fY+3.f, 0, 0);
	m_NumFont->DrawText(NULL, NowNum, -1, &ItemNum, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f));
	if (m_eCollision == TYPE_ON)
	{
		Set_Notice();
	}

	if (m_eCollision == TYPE_NO)
	{
		CUIManager* pUIInstance = CUIManager::Get_Instance();
		if(pUIInstance->Check_Change(m_iTexturenum))
			pUIInstance->Set_ItemNoticeTextNum(99, false);
	}

	return S_OK;
}

HRESULT CConsumIcon::Set_ItemInfo(const _tchar * pTag)
{
	if (pTag == nullptr)
		return E_FAIL;
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	
	m_pItemInfo = pInvenInstance->Get_ItemInfo(pTag, CInvenManager::TYPE_CONSUM);
	m_pTag = m_pItemInfo->Get_ItemName();
	
	return S_OK;
}

void CConsumIcon::Set_NowNum(_uint iNum)
{
	m_pItemInfo->Set_NowNum(iNum);
}

void CConsumIcon::Set_Notice()
{
	CUIManager* pUIInstance = CUIManager::Get_Instance();
	switch (m_iTexturenum)
	{
	case 0:
		pUIInstance->Set_ItemNoticeTextNum(0, true);
		break;
	case 1:
		pUIInstance->Set_ItemNoticeTextNum(1, true);
		break;
	case 2:
		pUIInstance->Set_ItemNoticeTextNum(2, true);
		break;
	default:
		break;
	}
	
}

void CConsumIcon::Use_Item()
{
	switch (m_iTexturenum)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
	{
		CUIManager::Get_Instance()->Start_Loading();
		m_bSceneChange = true;
	}
		break;

	default:
		break;
	}
}

void CConsumIcon::Change_Texture()
{
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	if (pInvenInstance->Get_InvenType() == CInvenManager::TYPE_CONSUM)
	{
		if (pInvenInstance->Get_ItemInfo(m_pTag, CInvenManager::TYPE_CONSUM)->Get_NowNum() < 1)
		{
			m_iTexturenum = 99;
		}
		else
			m_iTexturenum = m_pItemInfo->Get_TextNum();


		CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
		if (m_eCollision == TYPE_DOWN && m_bRender)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragStart.mp3", 25, 1.f);
			pMouseInstance->Set_ItemIconIndex(CMouseManager::TYPE_ITEM, m_pTag, CInvenManager::TYPE_CONSUM, m_iTexturenum, m_pItemInfo->Get_ItemNotice(), m_UIInfo.iNum);
		}

		if (m_eCollision == TYPE_UP && m_bRender)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragEnd.mp3", 25, 1.f);
			CItemInfo* pTemp = pMouseInstance->Get_ItemInfo();
			pInvenInstance->Change_Info(m_pTag, pMouseInstance->Get_Indexnum(), CInvenManager::TYPE_CONSUM);
			m_pItemInfo = pTemp;
			m_pTag = pTemp->Get_ItemName();
		}
	}
}

CConsumIcon* CConsumIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CConsumIcon*		pInstance = new CConsumIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CConsumIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CConsumIcon::Clone(void * pArg)
{
	CConsumIcon*		pInstance = new CConsumIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CConsumIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CConsumIcon::Free()
{
	__super::Free();
}

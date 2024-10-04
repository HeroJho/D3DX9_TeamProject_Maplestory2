#include "stdafx.h"
#include "..\Public\InvenManager.h"

#include "GameInstance.h"

#include "UI.h"
#include "Inventory.h"
#include "ItemInfo.h"
#include "Item.h"

IMPLEMENT_SINGLETON(CInvenManager)

CInvenManager::CInvenManager()
{
	m_eInvenType = TYPE_CONSUM;
}

HRESULT CInvenManager::Set_Inventory(CInventory* pInventory)
{
	if (pInventory == nullptr)
		return E_FAIL;

	m_pInventory = pInventory;
	Safe_AddRef(m_pInventory);

	return S_OK;
}

HRESULT CInvenManager::Add_InvenImage(CUI* pInvenImage)
{
	if (pInvenImage == nullptr)
		return E_FAIL;

	m_pInventory->Add_InvenImage(pInvenImage);

	return S_OK;
}

HRESULT CInvenManager::Add_InvenIcon(InvenType eType, CUI * pInvenIcon)
{
	if (pInvenIcon == nullptr)
		return E_FAIL;

	m_pInventory->Add_InvenIcon(eType, pInvenIcon);

	return S_OK;
}

HRESULT CInvenManager::Set_InvenType(InvenType eType)
{
	m_eInvenType = eType;
	return S_OK;
}

HRESULT CInvenManager::Add_ItemInfo(const _tchar * pTag, InvenType eType, CItemInfo * pItemInfo)
{
	if (pItemInfo == nullptr)
		return E_FAIL;

	m_ItemInfo[eType].emplace(pTag, pItemInfo);


	return S_OK;
}


CItemInfo * CInvenManager::Get_ItemInfo(const _tchar* pTag, InvenType eType)
{
	return 	Find_ItemInfo(pTag, eType);
}

CItemInfo * CInvenManager::Find_ItemInfo(const _tchar* pTag, InvenType eType)
{
	auto	iter = find_if(m_ItemInfo[eType].begin(), m_ItemInfo[eType].end(), CTag_Finder(pTag));

	if (iter == m_ItemInfo[eType].end())
		return nullptr;

	return iter->second;
}

HRESULT CInvenManager::Check_Icon(const _tchar* pTag, InvenType eType, _uint iNum)
{
	if (pTag == nullptr)
		return E_FAIL;

	m_pInventory->Set_Icon(pTag, eType, iNum);
	
	return S_OK;
}

HRESULT CInvenManager::Change_Info(const _tchar * pTag, _uint iIndex, InvenType eType)
{
	return m_pInventory->Change_Info(pTag, iIndex, eType);
}


void CInvenManager::MakeItem(InvenType eType, _int TextNum, const _tchar * pTag, _float3 vPos, LEVEL eLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CItem::ITEMINFO ItemInfo;
	ItemInfo.eType = eType;
	ItemInfo.iTextNum = TextNum;
	ItemInfo.pTag = pTag;
	ItemInfo.vPos = vPos;

	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), eLevel, TEXT("Layer_Item"), &ItemInfo);

	Safe_Release(pGameInstance);
}

void CInvenManager::Free()
{
	for (size_t i = TYPE_EQUIP; i < TYPE_END - 1; ++i)
	{
		for (auto& pair : m_ItemInfo[i])
		{
			Safe_Release(pair.second);
		}
	}
	m_ItemInfo->clear();
	Safe_Release(m_pInventory);
}



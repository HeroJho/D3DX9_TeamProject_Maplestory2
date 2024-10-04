#include "stdafx.h"
#include "..\Public\QuickSlotManager.h"
#include "QuickSlotSkill.h"
#include "QuickSlotItem.h"

IMPLEMENT_SINGLETON(CQuickSlotManager)

CQuickSlotManager::CQuickSlotManager()
{
}

HRESULT CQuickSlotManager::Add_QuickSlotSkill(CQuickSlotSkill* pQuickSlot)
{
	if (pQuickSlot == nullptr)
		return E_FAIL;
	m_QuickSlotSkill.push_back(pQuickSlot);
	
	Safe_AddRef(pQuickSlot);

	return S_OK;
}

HRESULT CQuickSlotManager::Add_QuickSlotItem(CQuickSlotItem * pQuickSlot)
{
	if (pQuickSlot == nullptr)
		return E_FAIL;
	m_QuickSlotItem.push_back(pQuickSlot);

	Safe_AddRef(pQuickSlot);

	return S_OK;
}

_bool CQuickSlotManager::Check_SkillTexture(_uint iTextnum)
{
	for (auto iter : m_QuickSlotSkill)
	{
		if (iter->Get_TextNum() == iTextnum)
			return true;
	}

	return false;
}

_bool CQuickSlotManager::Check_ItemTexture(_uint iTextnum)
{
	for (auto iter : m_QuickSlotItem)
	{
		if (iter->Get_TextNum() == iTextnum)
			return true;
	}

	
	return false;
}

HRESULT CQuickSlotManager::Change_SkillSlot(_uint iIndexnum, CQuickSlotSkill* pQuickSlot)
{
	
	for (auto iter : m_QuickSlotSkill)
	{
		if (iter->Get_IndexNum() == iIndexnum)
		{
			if (iIndexnum == 99)
			{
				iter->Set_Data(99, nullptr, CSkillManager::GRADE_END, nullptr);
			}
			iter->Set_Data(pQuickSlot->Get_TextNum(), pQuickSlot->Get_Tag(), pQuickSlot->Get_Grade(), pQuickSlot->Get_Notice());
		}
	}
	return S_OK;
}

HRESULT CQuickSlotManager::Change_ItemSlot(_uint iIndexnum, CQuickSlotItem * pQuickSlot)
{
	for (auto iter : m_QuickSlotItem)
	{
		if (iter->Get_IndexNum() == iIndexnum)
		{
			if (iIndexnum == 99)
			{
				iter->Set_Data(99, nullptr, CInvenManager::TYPE_END, nullptr);
			}
			iter->Set_Data(pQuickSlot->Get_TextNum(), pQuickSlot->Get_Tag(), pQuickSlot->Get_Type(), pQuickSlot->Get_Notice());
		}
	}
	return S_OK;
}

void CQuickSlotManager::Clear_Data(_uint iIndexnum)
{
	for (auto iter : m_QuickSlotSkill)
	{
		if (iter->Get_IndexNum() == iIndexnum)
		{
			iter->Clear_Data();
		}
	}

	for (auto iter : m_QuickSlotItem)
	{
		if (iter->Get_IndexNum() == iIndexnum)
		{
			iter->Clear_Data();
		}
	}
}

_bool CQuickSlotManager::Check_Delete()
{
	for (auto iter : m_QuickSlotSkill)
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);		
		if(PtInRect(&iter->Get_RectUI(), ptMouse))
			return false;
	}

	for (auto iter : m_QuickSlotItem)
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		if (PtInRect(&iter->Get_RectUI(), ptMouse))
			return false;
	}
	return true;
}

void CQuickSlotManager::Free()
{
	for (auto iter : m_QuickSlotSkill)
	{
		Safe_Release(iter);
	}
	m_QuickSlotSkill.clear();

	for (auto iter : m_QuickSlotItem)
	{
		Safe_Release(iter);
	}
	m_QuickSlotItem.clear();
}

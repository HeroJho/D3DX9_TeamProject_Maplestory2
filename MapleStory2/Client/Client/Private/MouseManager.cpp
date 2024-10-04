#include "stdafx.h"
#include "..\Public\MouseManager.h"
#include "MouseSkillIcon.h"
#include "MouseItemIcon.h"
#include "SkillInfo.h"
#include "ItemInfo.h"

IMPLEMENT_SINGLETON(CMouseManager)

CMouseManager::CMouseManager()
{
	m_eGrade = CSkillManager::GRADE_BEGENNER;
	m_eType = CInvenManager::TYPE_END;
}

HRESULT CMouseManager::Add_pMouseSkillIcon(CMouseSkillIcon* pMouseSkillIcon)
{
	if (pMouseSkillIcon == nullptr)
		return E_FAIL;

	m_pMouseSkillIcon = pMouseSkillIcon;

	return S_OK;
}

void CMouseManager::Set_SkillIconIndex(PickType ePick, const _tchar* pSkillInfoTag, CSkillManager::SkillGrade eGrade, _uint iText, const wchar_t* pNotice, _uint iIndex)
{

	m_ePick = ePick;
	m_pSkillInfoTag = pSkillInfoTag;
	m_eGrade = eGrade;
	m_pMouseSkillIcon->Set_TextureNum(iText);
	m_cSkillNotice = pNotice;
	m_iIndexNum = iIndex;

}

_uint CMouseManager::Get_SkillIconTextnum()
{
	return m_pMouseSkillIcon->Get_TextNum();
}


CSkillInfo * CMouseManager::Get_SkillInfo()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	CSkillInfo* pSkillInfo = pSkillInstance->Get_SkillInfo(m_pSkillInfoTag, m_eGrade);
	
	return pSkillInfo;
}

HRESULT CMouseManager::Add_pMouseItemIcon(CMouseItemIcon * pMouseItemIcon)
{
	if (pMouseItemIcon == nullptr)
		return E_FAIL;

	m_pMouseItemIcon = pMouseItemIcon;

	return S_OK;
}

void CMouseManager::Set_ItemIconIndex(PickType ePick, const _tchar * pItemInfoTag, CInvenManager::InvenType eType, _uint iText, const wchar_t * pNotice, _uint iIndex)
{
	m_ePick = ePick;
	m_pItemInfoTag = pItemInfoTag;
	m_eType = eType;
	m_pMouseItemIcon->Set_Type(eType);
	m_pMouseItemIcon->Set_TextureNum(iText);
	m_cItemNotice = pNotice;
	m_iIndexNum = iIndex;
}

_uint CMouseManager::Get_ItemIconTextnum()
{
	return m_pMouseItemIcon->Get_TextNum();
}

CItemInfo * CMouseManager::Get_ItemInfo()
{
	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	CItemInfo* pItemInfo = pInvenInstance->Get_ItemInfo(m_pItemInfoTag, m_eType);

	return pItemInfo;
}

void CMouseManager::Free()
{

}

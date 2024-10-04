#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "SkillManager.h"
#include "InvenManager.h"

BEGIN(Client)
class CMouseItemIcon;
class CMouseSkillIcon;
class CUI;
class CSkillInfo;

class CMouseManager final : public CBase
{
	DECLARE_SINGLETON(CMouseManager)
public:
	enum PickType {TYPE_SKILL, TYPE_ITEM, TYPE_QUICK, TYPE_INVENTORY, TYPE_END};
public:
	CMouseManager();
	virtual ~CMouseManager() = default;


public:
	HRESULT Add_pMouseSkillIcon(CMouseSkillIcon* pMouseSkillIcon);
	void Set_SkillIconIndex(PickType ePick, const _tchar* pSkillInfoTag, CSkillManager::SkillGrade eGrade , _uint iText, const wchar_t* pNotice, _uint iIndex = 99);
	_uint Get_SkillIconTextnum();
	const _tchar* Get_SkillInfoTag() { return m_pSkillInfoTag; }
	CSkillManager::SkillGrade Get_Grade() { return m_eGrade; }
	CSkillInfo* Get_SkillInfo();
	HRESULT Add_pMouseItemIcon(CMouseItemIcon* pMouseItemIcon);
	void Set_ItemIconIndex(PickType ePick, const _tchar* pItemInfoTag, CInvenManager::InvenType eType, _uint iTextNum, const wchar_t* pNotice, _uint iIndex = 99);
	_uint Get_ItemIconTextnum();
	const _tchar* Get_ItemInfoTag() { return m_pItemInfoTag; }
	CInvenManager::InvenType Get_Type() { return m_eType; }
	CItemInfo* Get_ItemInfo();

	_uint Get_Indexnum() { return m_iIndexNum; }
	const wchar_t* Get_SkillNotice() { return m_cSkillNotice; }
	const wchar_t* Get_ItemNotice() { return m_cItemNotice; }
	PickType Get_PickType() { return m_ePick; }
	void Set_PrickType(PickType ePick) { m_ePick = ePick; }
	

private:
	CMouseSkillIcon* m_pMouseSkillIcon;
	const _tchar* m_pSkillInfoTag;
	CSkillManager::SkillGrade m_eGrade;
	const wchar_t* m_cSkillNotice;
	CMouseItemIcon* m_pMouseItemIcon;
	const _tchar* m_pItemInfoTag;
	CInvenManager::InvenType m_eType;
	const wchar_t* m_cItemNotice;
	_uint m_iIndexNum;
	PickType m_ePick;
public:
	virtual void Free() override;

};
END


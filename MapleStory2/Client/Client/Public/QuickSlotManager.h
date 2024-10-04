#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CQuickSlotSkill;
class CQuickSlotItem;

class CQuickSlotManager final : public CBase
{
	DECLARE_SINGLETON(CQuickSlotManager)
public:
	CQuickSlotManager();
	virtual ~CQuickSlotManager() = default;

public:
	HRESULT Add_QuickSlotSkill(CQuickSlotSkill* pQuickSlot);
	HRESULT Add_QuickSlotItem(CQuickSlotItem* pQuickSlot);
	_bool Check_SkillTexture(_uint iTextnum);
	_bool Check_ItemTexture(_uint iTextnum);
	HRESULT Change_SkillSlot(_uint iIndexnum, CQuickSlotSkill* pQuickSlot);
	HRESULT Change_ItemSlot(_uint iIndexnum, CQuickSlotItem* pQuickSlot);
	void Clear_Data(_uint iIndexnum);
	_bool Check_Delete();

private:
	list<CQuickSlotSkill*> m_QuickSlotSkill;
	list<CQuickSlotItem*> m_QuickSlotItem;


public:
	virtual void Free() override;

};
END


#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CUI;
class CInventory;
class CItemInfo;
class CItem;

class CInvenManager final : CBase
{
	DECLARE_SINGLETON(CInvenManager)
public:
	enum InvenType { TYPE_EQUIP, TYPE_CONSUM, TYPE_STUFF,TYPE_INSTALL, TYPE_CASH, TYPE_END};

private:
	CInvenManager();
	virtual ~CInvenManager() = default;

public:
	HRESULT Set_Inventory(CInventory* pInvenFrame);

	HRESULT Add_InvenImage(CUI* pInvenImage);
	HRESULT Add_InvenIcon(InvenType eType, CUI* pInvenIcon);

	HRESULT Set_InvenType(InvenType eType);
	InvenType Get_InvenType() { return m_eInvenType; }

	HRESULT Add_ItemInfo(const _tchar* pTag, InvenType eType, CItemInfo* pItemInfo);
	CItemInfo* Get_ItemInfo(const _tchar* pTag, InvenType eType);
	CItemInfo* Find_ItemInfo(const _tchar* pTag, InvenType eType);
	HRESULT Check_Icon(const _tchar* pTag, InvenType eType, _uint iNum);
	HRESULT Change_Info(const _tchar* pTag, _uint iIndex, InvenType eType);
	
	void MakeItem(InvenType eType, _int TextNum, const _tchar* pTag, _float3 vPos, LEVEL eLevel);

	
private:
	CInventory* m_pInventory;
	InvenType m_eInvenType;
	map<const _tchar*, CItemInfo*> m_ItemInfo[TYPE_END];

public:
	virtual void Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Client)
class CItemInfo : public CBase
{
protected:
	CItemInfo();
	virtual ~CItemInfo() = default;

public:
	const _uint Get_NowNum() { return m_iNowNum; }
	void Set_NowNum(_uint iNum) { m_iNowNum += iNum; }
	const wchar_t* Get_ItemName() { return m_pItemName; }
	const wchar_t* Get_ItemNotice() { return m_pItemNotice; }
	void Set_ItemNotice(const wchar_t* pItemNotice) { m_pItemNotice = pItemNotice; }
	const _uint Get_TextNum() { return m_iTextNum; }

public:
	virtual void Free() override;

protected:
	const wchar_t* m_pItemName;
	const wchar_t* m_pItemNotice;
	_uint m_iNowNum;
	_uint m_iMaxNum;
	_uint m_iTextNum;
};


END


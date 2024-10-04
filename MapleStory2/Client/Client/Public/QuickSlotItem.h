#pragma once
#include "UI.h"
#include "InvenManager.h"
BEGIN(Client)
class CQuickSlotItem final : public  CUI
{
private:
	CQuickSlotItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuickSlotItem(const CQuickSlotItem& rhs);
	virtual ~CQuickSlotItem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();
	_uint Get_TextNum() { return m_iTexturenum; }
	_uint Get_IndexNum() { return m_iIndexNum; }
	const _tchar* Get_Tag() { return m_pItemInfoTag; }
	const wchar_t* Get_Notice() { return m_pItemNotice; }
	CInvenManager::InvenType Get_Type() { return m_eType; }
	void Clear_Data();
	void Set_Data(_uint Textnum, const _tchar* Tag, CInvenManager::InvenType eType, const wchar_t* pNotice) { m_iTexturenum = Textnum, m_pItemInfoTag = Tag, m_eType = eType, m_pItemNotice = pNotice; }


private:
	_uint m_iIndexNum;
	const _tchar* m_pItemInfoTag;
	CInvenManager::InvenType m_eType;
	const wchar_t* m_pItemNotice;
	LPD3DXFONT m_NowNumFont;
	LPD3DXFONT m_NoticeFont;


public:
	static CQuickSlotItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


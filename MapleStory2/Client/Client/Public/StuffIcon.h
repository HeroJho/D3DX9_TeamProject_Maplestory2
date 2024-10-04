#pragma once
#include "UI.h"
#include "ItemInfo.h"

BEGIN(Client)
class CStuffIcon final : public  CUI
{
private:
	CStuffIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStuffIcon(const CStuffIcon& rhs);
	virtual ~CStuffIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Set_ItemInfo(const _tchar* pTag);
	const _tchar* Get_Tag() { return m_pTag; }
	void Set_NowNum(_uint iNum);
public:
	void Change_Texture();
	void Set_Notice();

public:
	static CStuffIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	CItemInfo* m_pItemInfo;
	LPD3DXFONT m_NumFont;
	_uint m_iIndexNum;
	const _tchar* m_pTag;
};
END


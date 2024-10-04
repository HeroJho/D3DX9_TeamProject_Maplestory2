#pragma once
#include "UI.h"

BEGIN(Client)
class CQuickSlot final : public  CUI
{
private:
	CQuickSlot(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuickSlot(const CQuickSlot& rhs);
	virtual ~CQuickSlot() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Draw_KeyText();
private:
	LPD3DXFONT m_KeyFont;
	_uint m_iIndexNum;
public:
	static CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


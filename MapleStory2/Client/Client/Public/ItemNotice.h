#pragma once
#include "UI.h"

BEGIN(Client)
class CItemNotice final : public  CUI
{
private:
	CItemNotice(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemNotice(const CItemNotice& rhs);
	virtual ~CItemNotice() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void RenderNotice();
	void RedPortionNotice();
	void BluePortionNotice();
	void BlueSnailNotice();
	void RedSnailNotice();
	void WoodNotice();
	void SlimeNotice();
	void RibbonNotice();
	void GreenmushroomNotice();
	void TeleportNotice();

public:
	static CItemNotice* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	LPD3DXFONT m_TitleFont;
	LPD3DXFONT m_TextFont;
	LPCSTR m_cTitle;
	LPCSTR m_cText;
};
END


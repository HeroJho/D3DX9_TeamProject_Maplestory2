#pragma once
#include "UI.h"

BEGIN(Client)
class CExpBar final : public  CUI
{
private:
	CExpBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CExpBar(const CExpBar& rhs);
	virtual ~CExpBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CExpBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iExp;
	_uint m_iExpDigit;
	_uint m_iLevel;
	LPD3DXFONT m_ExpFont;
	LPD3DXFONT m_LevelFont;
};
END


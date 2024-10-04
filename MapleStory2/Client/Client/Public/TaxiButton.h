#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CTaxiButton final : public CUI
{
public:
	typedef struct tagTaxiButtonDesc
	{
		_float2 vPos;
		LEVEL eLevel;
	}TAXIBUTTONDESC;

private:
	CTaxiButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTaxiButton(const CTaxiButton& rhs);
	virtual ~CTaxiButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	HRESULT SetUp_Components();


private:
	TAXIBUTTONDESC m_Desc;

	LPD3DXFONT m_pFont;
	LPD3DXFONT m_pTipFont;

	D3DXCOLOR m_OnColor;

	_float m_fTimeAcc = 0.f;
	_bool m_bTrigger = false;

public:
	static CTaxiButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


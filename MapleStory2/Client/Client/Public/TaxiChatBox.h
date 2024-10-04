#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CTaxiChatBox final : public CUI
{
private:
	CTaxiChatBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTaxiChatBox(const CTaxiChatBox& rhs);
	virtual ~CTaxiChatBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void Start_Chat();
	void End_Chat();



private:
	HRESULT SetUp_Components();
	void On_Chat(_float fTimeDelta);
	void Off_Chat(_float fTimeDelta);



private:
	_float m_fTimeAcc = 0.f;
	_int m_iCount = 0;

	_bool m_bOn = false;


	list<class CTaxiButton*> m_Buttons;

	LPD3DXFONT m_pFont;
	LPD3DXFONT m_pNameFont;


public:
	static CTaxiChatBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


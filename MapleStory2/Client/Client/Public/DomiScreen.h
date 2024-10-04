#pragma once
#include "Client_Defines.h"

#include "UI.h"

BEGIN(Client)

class CDomiScreen final : public CUI
{
private:
	CDomiScreen(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDomiScreen(const CDomiScreen& rhs);
	virtual ~CDomiScreen() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void OnCutScreen();
	void OffCutScreen();

private:
	HRESULT SetUp_Components();
	void OnOffTick(_float fTimeDelta);
	void BlackMageTalkTick(_float fTimeDelta);
	void BlackMageTalk();

public:
	_float m_fTimeAcc = 0.f;
	_int m_iCount = 0;
	_bool m_bCutScreen = false;
	LPD3DXFONT m_pFont;

	_float m_fTalkTimeAcc = 0.f;
	_int m_iTalkCount = 0;
	_float m_fTempA = 1.f;

	_bool m_bStartTalk = false;

	_float m_fTalkDelayTimeAcc = 0.f;
	_bool m_bOnTalk = false;

public:
	static CDomiScreen* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


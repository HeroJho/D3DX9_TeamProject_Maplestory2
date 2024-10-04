#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLoading final : public CUI
{
private:
	CLoading(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading(const CLoading& rhs);
	virtual ~CLoading() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void StartLoading();
	void EndLoading();

private:
	void TickStart(_float fTimeDelta);
	void TickEnd(_float fTimeDelta);

private:
	HRESULT SetUp_Components();

private:
	_float m_fTimeAcc = 0.f;
	_float m_fTime = 0.f;
	_float m_fCount = 0.f;
	_float m_fMaxCount = 0.f;

	_bool m_bOn = false;

	class CLoading_Mop* m_pMop = nullptr;
	_bool m_bMop = false;



	LPD3DXFONT m_pFont;
	LPD3DXFONT m_pTipFont;

	_float m_fTimeRenAcc = 0.f;
	_int m_fRanNum = 0.f;

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


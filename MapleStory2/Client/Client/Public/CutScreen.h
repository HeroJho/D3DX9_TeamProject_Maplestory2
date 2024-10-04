#pragma once
#include "Client_Defines.h"

#include "UI.h"

BEGIN(Client)

class CCutScreen final : public CUI
{
private:
	CCutScreen(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCutScreen(const CCutScreen& rhs);
	virtual ~CCutScreen() = default;

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


public:
	_float m_fTimeAcc = 0.f;
	_int m_iCount = 0;
	_bool m_bCutScreen = false;

public:
	static CCutScreen* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


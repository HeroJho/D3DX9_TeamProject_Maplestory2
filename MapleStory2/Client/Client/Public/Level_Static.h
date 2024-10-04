#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Static final : public CLevel
{
private:
	CLevel_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Static() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);


public:
	static CLevel_Static* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END


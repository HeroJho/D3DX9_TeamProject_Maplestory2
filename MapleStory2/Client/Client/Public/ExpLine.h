#pragma once
#include "UI.h"

BEGIN(Client)
class CExpLine final : public  CUI
{
private:
	CExpLine(LPDIRECT3DDEVICE9 pGraphic_Device);
	CExpLine(const CExpLine& rhs);
	virtual ~CExpLine() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CExpLine* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


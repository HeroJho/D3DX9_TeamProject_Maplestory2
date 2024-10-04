#pragma once
#include "UI.h"

BEGIN(Client)
class CExpBase final : public  CUI
{
private:
	CExpBase(LPDIRECT3DDEVICE9 pGraphic_Device);
	CExpBase(const CExpBase& rhs);
	virtual ~CExpBase() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CExpBase* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


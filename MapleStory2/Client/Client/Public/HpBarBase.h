#pragma once
#include "UI.h"

BEGIN(Client)
class CHpBarBase final : public  CUI
{
private:
	CHpBarBase(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHpBarBase(const CHpBarBase& rhs);
	virtual ~CHpBarBase() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CHpBarBase* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


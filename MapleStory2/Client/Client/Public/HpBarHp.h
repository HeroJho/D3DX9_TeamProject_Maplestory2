#pragma once
#include "UI.h"

BEGIN(Client)
class CHpBarHp final : public  CUI
{
private:
	CHpBarHp(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHpBarHp(const CHpBarHp& rhs);
	virtual ~CHpBarHp() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CHpBarHp* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


#pragma once
#include "UI.h"

BEGIN(Client)
class CHpBarMp final : public  CUI
{
private:
	CHpBarMp(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHpBarMp(const CHpBarMp& rhs);
	virtual ~CHpBarMp() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CHpBarMp* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


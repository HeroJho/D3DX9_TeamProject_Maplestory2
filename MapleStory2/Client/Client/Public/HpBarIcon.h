#pragma once
#include "UI.h"

BEGIN(Client)
class CHpBarIcon final : public  CUI
{
private:
	CHpBarIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHpBarIcon(const CHpBarIcon& rhs);
	virtual ~CHpBarIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CHpBarIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


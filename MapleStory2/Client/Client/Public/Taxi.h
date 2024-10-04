#pragma once

#include "Creature.h"
#include "Bulb.h"

BEGIN(Client)

class CTaxi final : public CCreature
{
private:
	CTaxi(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTaxi(const CTaxi& rhs);
	virtual ~CTaxi() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

private:
	void Tick_Idle(_float fTimeDelta);
	void Tick_Move(_float fTimeDelta);
	void Tick_Hit(_float fTimeDelta);
	void Tick_Chase(_float fTimeDelta);

private:
	HRESULT SetUp_Components();


public:
	static CTaxi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once

#include "Creature.h"

BEGIN(Client)

class CBeastHit final : public CCreature
{

private:
	CBeastHit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBeastHit(const CBeastHit& rhs);
	virtual ~CBeastHit() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

protected:
	virtual HRESULT Set_RenderState() override;
	virtual HRESULT Reset_RenderState() override;

public:
	virtual void SetAni() override;

private:
	HRESULT SetUp_Components();

public:
	static CBeastHit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
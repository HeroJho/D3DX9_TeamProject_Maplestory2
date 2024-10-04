#pragma once

#include "Creature.h"

BEGIN(Client)

class CChasingShotHit final : public CCreature
{

private:
	CChasingShotHit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChasingShotHit(const CChasingShotHit& rhs);
	virtual ~CChasingShotHit() = default;

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
	static CChasingShotHit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
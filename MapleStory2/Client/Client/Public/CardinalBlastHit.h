#pragma once

#include "Creature.h"

BEGIN(Client)

class CCardinalBlastHit final : public CCreature
{

private:
	CCardinalBlastHit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCardinalBlastHit(const CCardinalBlastHit& rhs);
	virtual ~CCardinalBlastHit() = default;

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
	static CCardinalBlastHit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
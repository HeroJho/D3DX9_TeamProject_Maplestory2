#pragma once

#include "Creature.h"

BEGIN(Client)

class CBlackWizardHit final : public CCreature
{

private:
	CBlackWizardHit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizardHit(const CBlackWizardHit& rhs);
	virtual ~CBlackWizardHit() = default;

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

private:
	HRESULT SetUp_Components();

public:
	static CBlackWizardHit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
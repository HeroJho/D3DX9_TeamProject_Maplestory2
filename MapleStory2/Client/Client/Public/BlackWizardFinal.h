#pragma once

#include "Creature.h"

BEGIN(Client)

class CBlackWizardFinal final : public CCreature
{

private:
	CBlackWizardFinal(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizardFinal(const CBlackWizardFinal& rhs);
	virtual ~CBlackWizardFinal() = default;

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
	CTransform* m_pTarget;
	list<CGameObject*> m_pOther;
private:
	HRESULT SetUp_Components();

public:
	static CBlackWizardFinal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
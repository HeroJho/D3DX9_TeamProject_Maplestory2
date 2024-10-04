#pragma once

#include "Creature.h"

BEGIN(Client)

class CBlackWizardAttack1 final : public CCreature
{

private:
	CBlackWizardAttack1(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizardAttack1(const CBlackWizardAttack1& rhs);
	virtual ~CBlackWizardAttack1() = default;

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
	bool m_bSound;
	bool m_bSound2;
private:
	HRESULT SetUp_Components();

public:
	static CBlackWizardAttack1* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
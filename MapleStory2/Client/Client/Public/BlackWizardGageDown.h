#pragma once

#include "Creature.h"

BEGIN(Client)

class CBlackWizardGageDown final : public CCreature
{

private:
	CBlackWizardGageDown(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizardGageDown(const CBlackWizardGageDown& rhs);
	virtual ~CBlackWizardGageDown() = default;

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

	void SetAni();

private:
	list<CGameObject*> m_pOther;
private:
	HRESULT SetUp_Components();
	
public:
	static CBlackWizardGageDown* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
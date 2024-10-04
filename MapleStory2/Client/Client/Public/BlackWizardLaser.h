#pragma once

#include "Creature.h"

BEGIN(Client)

class CBlackWizardLaser final : public CCreature
{
public:
	enum STATE {STATE_REGEN, STATE_IDLE, STATE_DIE, STATE_END};
private:
	CBlackWizardLaser(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizardLaser(const CBlackWizardLaser& rhs);
	virtual ~CBlackWizardLaser() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

//protected:
//	virtual HRESULT Set_RenderState() override;
//	virtual HRESULT Reset_RenderState() override;

	void SetAni();

private:
	CTransform* m_pTarget;
	list<CGameObject*> m_pOther;
private:
	HRESULT SetUp_Components();
	STATE m_eCurState;
	bool m_bMove;
public:
	static CBlackWizardLaser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
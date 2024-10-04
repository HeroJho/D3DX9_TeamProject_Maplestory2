#pragma once

#include "Creature.h"

BEGIN(Client)

class CBlackWizard final : public CCreature
{
public:
	enum STATE {STATE_APPEAR, STATE_ESCAPE, STATE_STAND, STATE_SKILL1, STATE_SKILL2,STATE_SKILL3, STATE_SKILL4,  STATE_END};
public:
	CBlackWizard(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizard(const CBlackWizard& rhs);
	virtual ~CBlackWizard() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

public:
	void SetState();
	void Pattern();
	void Fix_Scale();

	void MakeBlockCube();

public:
	virtual void SetAni() override;
	virtual void Damaged(CGameObject* pOther) override;
	void Set_Final() { m_bFinal = true; }
	void Skill3();
	void Skill4();
protected:
	virtual HRESULT Set_RenderState() override;
	virtual HRESULT Reset_RenderState() override;

private:
	HRESULT SetUp_Components();

private:
	CTransform* m_pTarget;
	STATE m_eCurState;
	_float m_fDead = 0.f;
	bool m_bState;
	bool m_bFinal;
	bool m_bSound;
	bool m_bSceneEnd;
	bool m_bDead = false;
	_float m_fSceneAcc;
	_int m_iHp;

public:
	static CBlackWizard* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


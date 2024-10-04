#pragma once

#include "Creature.h"

BEGIN(Client)

class CBeastAttack final : public CCreature
{
public:
	typedef struct tagBeastAttackDESC
	{
		DIR eDir;
		CTransform* pTransform;
	}BEASTATTACKDESC;

private:
	CBeastAttack(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBeastAttack(const CBeastAttack& rhs);
	virtual ~CBeastAttack() = default;

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
	void SetState(STATE eState, DIR eDir);
	void SetPosition(DIR eDir);
public:
	virtual void SetAni() override;


private:
	STATE m_eCurState;
	DIR m_eDir;
	CTransform* m_pTarget;
	BEASTATTACKDESC m_Desc;
	bool m_bCreate;
	bool m_bSound;

	list<CGameObject*> m_pOther;
private:
	HRESULT SetUp_Components();

public:
	static CBeastAttack* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
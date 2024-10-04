#pragma once

#include "Creature.h"

BEGIN(Client)

class CWarriorReef final : public CCreature
{
public:
	typedef struct tagWarriorReefDESC
	{
		DIR eDir;
	}WARRIORREEFDESC;

private:
	CWarriorReef(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWarriorReef(const CWarriorReef& rhs);
	virtual ~CWarriorReef() = default;

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
	void SetDirection();
	void SetPosition(DIR eDir);
public:
	virtual void SetAni() override;


private:
	STATE m_eCurState;
	DIR m_eDir;
	_bool m_bRender;
	CTransform* m_pTarget;
	WARRIORREEFDESC m_Desc;
private:
	HRESULT SetUp_Components();

public:
	static CWarriorReef* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
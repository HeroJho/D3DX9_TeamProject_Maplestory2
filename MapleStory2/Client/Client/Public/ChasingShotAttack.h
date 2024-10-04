#pragma once

#include "Creature.h"

BEGIN(Client)

class CChasingShotAttack final : public CCreature
{
public:
	typedef struct tagChasingShotAttackDESC
	{
		DIR eDir;
	}CHASINGATTACKDESC;

private:
	CChasingShotAttack(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChasingShotAttack(const CChasingShotAttack& rhs);
	virtual ~CChasingShotAttack() = default;

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
	CTransform* m_pTarget;
	CHASINGATTACKDESC m_Desc;
	bool m_bCreate;
private:
	HRESULT SetUp_Components();

public:
	static CChasingShotAttack* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
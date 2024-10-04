#pragma once

#include "Creature.h"

BEGIN(Client)

class CChasingShotBullet final : public CCreature
{
public:
	typedef struct tagChasingShotBulletDESC
	{
		DIR eDir;
	}CHASINGBULLETDESC;
private:
	CChasingShotBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChasingShotBullet(const CChasingShotBullet& rhs);
	virtual ~CChasingShotBullet() = default;

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
	void MoveAttack(_float fTimeDelta);

public:
	virtual void SetAni() override;


private:
	STATE m_eCurState;
	DIR m_eDir;
	CHASINGBULLETDESC m_Desc;
	_float m_fDeadAcc;
	list<CGameObject*> m_pOther;

private:
	HRESULT SetUp_Components();

public:
	static CChasingShotBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once

#include "Creature.h"
#include "Item.h"

BEGIN(Client)

class CGreenMushroom final : public CCreature
{
private:
	enum RandomMove { MOVE_R, MOVE_L, MOVE_U, MOVE_D , MOVE_END};


	CGreenMushroom(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGreenMushroom(const CGreenMushroom& rhs);
	virtual ~CGreenMushroom() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

private:
	void Tick_Idle(_float fTimeDelta);
	void Tick_Move(_float fTimeDelta);
	void Tick_Hit(_float fTimeDelta);
	void Tick_Chase(_float fTimeDelta);
	void Tick_Die(_float fTimeDelta);

	void MakeItem();

public:
    virtual	void SetState(STATE eState, DIR eDir) override;

public:
	virtual void SetAni() override;
	virtual void Damaged(CGameObject* pOther) override;

private:
	STATE m_eCurState;
	DIR m_eDir;

	CGameObject* m_pTarget;

	_uint m_iMove;
	_float3 m_fEndPos;
	_float3 m_fStartPos;
	_bool m_bDir;
	_float m_fDistance;

	_int m_iHp;
	_int m_iIndexNum;

	_bool temp = false;

	_float m_fCountDead;

	CItem::ITEMINFO  GreenMushroomItem;

private:
	HRESULT SetUp_Components();
	void Die();

public:
	static CGreenMushroom* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
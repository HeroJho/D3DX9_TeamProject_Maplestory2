#pragma once

#include "Creature.h"

BEGIN(Client)

class CBlueMushmom final : public CCreature
{
private:
	CBlueMushmom(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlueMushmom(const CBlueMushmom& rhs);
	virtual ~CBlueMushmom() = default;

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
	void Tick_Attack(_float fTimeDelta);
	void Tick_Die(_float fTimeDelta);
	void Tick_CutScene(_float fTimeDelta);
	void Tick_End(_float fTimeDelta);
	void Cut_Attack(_float fTimeDelta);
	

public:
	void SetState(STATE eState, DIR eDir);

public:
	virtual void SetAni() override;
	virtual void Damaged(CGameObject* pOther) override;

private:
	HRESULT SetUp_Components();
	void Die();

private:
	STATE m_eCurState;
	DIR m_eDir;

	CGameObject* m_pTarget;


public:
	static CBlueMushmom* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_int	m_iHp;
	_int	m_iIndexNum;
	_float m_fCountDead;

	// 블루머쉬맘을 일정 거리만큼 점프시키는 변수
	_float m_fCountJump;
	_bool m_bAttack;

	_float m_fCountLanding;
	_bool m_bLanding;

	_float m_fCountLand;

	// 블루 머쉬맘이 점프하기 전 좌표
	_float3 vPos;

	// For.CutScene
	_float m_fCutTimeAcc = 0.f;
	_bool m_bShake = false;
};

END
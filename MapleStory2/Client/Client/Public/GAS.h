#pragma once

#include "Creature.h"
#include "Spawner.h"

BEGIN(Client)

class CGAS final : public CCreature
{
public:
	typedef struct tagGASDesc {
		_float3 vPos;
		STATE eState;
	}GASDESC;

private:
	CGAS(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGAS(const CGAS& rhs);
	virtual ~CGAS() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

private:
	void Tick_Idle(_float fTimeDelta);
	void Tick_Hit(_float fTimeDelta);
	void Tick_Chase(_float fTimeDelta);
	void Tick_Jump(_float fTimeDelta);
	void Tick_DJump(_float fTimeDelta);
	void Tick_Dash(_float fTimeDelta);
	void Tick_End(_float fTimeDelta);	// 텔포 이후
	void Tick_Attack(_float fTimeDelta);
	void Tick_Die(_float fTimeDelta);
	void Tick_Reset(_float fTimeDelta);
	void Tick_Return(_float fTimeDelta);
	void Tick_CutScene(_float fTimeDelta);
	void Tick_CutScene2(_float fTimeDelta);

public:
	void SetState(STATE eState, DIR eDir);

public:
	virtual void SetAni() override;
	virtual void Damaged(CGameObject* pOther) override;

	void DestroyCube(_int iLength);
	void MakeBlockCube(_float3 vPos);

private:
	HRESULT SetUp_Components();
	void Die();

private:
	STATE m_eCurState;
	DIR m_eDir;

	CGameObject* m_pTarget;


public:
	static CGAS* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
	void Reset_Hp() { m_iHp = 50; };

private:
	_int	m_iHp;
	_int	m_iIndexNum;
	_float m_fCountDead;

	_bool m_bPatterStart;
	_int m_iRandomPattern;
	_float m_fPatternCycle;

	_float m_fJump;
	_float m_fDJump;
	_float m_fAttack;
	_float m_fDash;
	_float m_fEnd;
	_float m_fReset;
	_float m_fReturn;

	_bool m_bTest;
	_bool m_bVanish;


	// For. CutScene
	_bool m_bCutSceneRend = false;
	_bool m_bCutSceneJump = false;
	_float m_fCutSceneTimeAcc = 0.f;
	


	_float m_iColliTime = 0;

	_bool m_bEffect;

	_bool m_bFirstPattern;
	_int m_iFirstPattern;


	_int m_iFirstSlime;
	_int m_iSlimePattern;
	_bool m_bSlime;


	_bool m_bAttackSound = false;
	_bool m_bDjumpSound = false;
	_bool m_bJumpSound = false;
	_bool m_bPatternSound = false;
	_bool m_bDashSound = false;
	_bool m_bReset = false;
	_bool m_bReturn = false;

	_bool m_bShake = false;
};

END
#pragma once

#include "Creature.h"

BEGIN(Client)

class CDamage final : public CCreature
{
public:
	enum DAMAGETYPE { DAMAGE_ATTACKED, DAMAGE_PLAYERATTAKED, DAMAGE_HEAL, DAMAGE_END };

public:
	typedef struct tagDamageDesc{
		DAMAGETYPE eType;
		string sDamage;
		_float3 vPos;
		_float fLifeTime;
	}DAMAGEDESC;
private:
	CDamage(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDamage(const CDamage& rhs);
	virtual ~CDamage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;


private:
	HRESULT SetUp_Components();
	HRESULT SetDamage(_char cNum);

private:
	DAMAGEDESC m_Desc;
	

	_float m_fTimeAcc = 0.f;

	_float m_fCulScale = 0.f;
	_float m_fScale = 1.f;
	_float m_fScaleAcc = 0.f;
	_float m_fTimeScaleAcc = 0.f;
	_float m_fInitScale = 0.f;
	_float m_fMddleScale = 0.f;

	_bool m_bDown = false;
	_bool m_bEnd = false;

public:
	static CDamage* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
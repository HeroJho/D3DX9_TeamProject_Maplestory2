#pragma once

#include "Creature.h"
#include "Damage.h"

BEGIN(Client)

class CDamageGen final : public CCreature
{
public:
	typedef struct tagDamageGenDesc {
		_int sMinDamage;
		_int sMaxDamage;
		_int iCount;
		_float fDelayTime;
		_float fLifeTime;
		_float3 vPos;
		_bool bIm;
		CDamage::DAMAGETYPE eType;
	}DAMAGEGENDESC;
private:
	CDamageGen(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDamageGen(const CDamageGen& rhs);
	virtual ~CDamageGen() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

	void Im(_float fTimeDelta);
	void NonIm(_float fTimeDelta);

private:
	HRESULT SetUp_Components();


private:
	DAMAGEGENDESC m_Desc;
	_float m_fTimeAcc = 0.f;
	_uint m_iCount = 0;

public:
	static CDamageGen* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
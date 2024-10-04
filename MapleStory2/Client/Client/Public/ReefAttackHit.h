#pragma once

#include "Creature.h"

BEGIN(Client)

class CReefAttackHit final : public CCreature
{
public:
	typedef struct tagReefAttackHitDESC
	{
		_float3 vPos;
	}REEFATTACKHITDESC;

private:
	CReefAttackHit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CReefAttackHit(const CReefAttackHit& rhs);
	virtual ~CReefAttackHit() = default;

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

public:
	virtual void SetAni() override;


private:
	STATE m_eCurState;
	DIR m_eDir;
	REEFATTACKHITDESC m_Desc;

private:
	HRESULT SetUp_Components();

public:
	static CReefAttackHit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
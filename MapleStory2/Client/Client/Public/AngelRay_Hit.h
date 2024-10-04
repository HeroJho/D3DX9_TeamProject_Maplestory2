#pragma once

#include "Creature.h"

BEGIN(Client)

class CAngelRay_Hit final : public CCreature
{
public:
	typedef struct tagAngleRay_HitDESC
	{
		_float3 vPos;
	}ANGELHITDESC;

private:
	CAngelRay_Hit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAngelRay_Hit(const CAngelRay_Hit& rhs);
	virtual ~CAngelRay_Hit() = default;

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

private:
	void Tick_Idle(_float fTimeDelta);
	void Tick_Move(_float fTimeDelta);
	void Tick_Hit(_float fTimeDelta);

public:
	void SetState(STATE eState, DIR eDir);

public:
	virtual void SetAni() override;


private:
	STATE m_eCurState;
	DIR m_eDir;
	ANGELHITDESC m_Desc;

private:
	HRESULT SetUp_Components();

public:
	static CAngelRay_Hit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
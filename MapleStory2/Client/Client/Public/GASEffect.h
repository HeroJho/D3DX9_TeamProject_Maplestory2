#pragma once

#include "Creature.h"

BEGIN(Client)

class CGASEffect final : public CCreature
{
public:
	typedef struct tagSunCross_HitDESC
	{
		_float3 vPos;
	}SUNCROSSHITDESC;

private:
	CGASEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGASEffect(const CGASEffect& rhs);
	virtual ~CGASEffect() = default;

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
	SUNCROSSHITDESC m_Desc;

	_float m_fEffectCount;

private:
	HRESULT SetUp_Components();

public:
	static CGASEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
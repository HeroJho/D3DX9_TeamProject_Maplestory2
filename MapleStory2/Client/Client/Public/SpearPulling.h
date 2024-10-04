#pragma once

#include "Creature.h"

BEGIN(Client)

class CSpearPulling final : public CCreature
{
public:
	typedef struct tagSpearPullingDESC
	{
		DIR eDir;
	}SPEARPULLINGDESC;

private:
	CSpearPulling(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpearPulling(const CSpearPulling& rhs);
	virtual ~CSpearPulling() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;
	void Pulling(CGameObject* pOther);

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
	_bool m_bPulling;
	_float m_fPullAcc;
	CTransform* m_pTarget;
	SPEARPULLINGDESC m_Desc;

	list<CGameObject*> m_pOther;
private:
	HRESULT SetUp_Components();

public:
	static CSpearPulling* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
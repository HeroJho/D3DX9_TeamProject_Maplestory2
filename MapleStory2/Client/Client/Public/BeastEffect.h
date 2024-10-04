#pragma once

#include "Creature.h"

BEGIN(Client)

class CBeastEffect final : public CCreature
{
public:
	typedef struct tagBeastEffectDESC
	{
		DIR eDir;
	}BEASTEFFECTDESC;

private:
	CBeastEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBeastEffect(const CBeastEffect& rhs);
	virtual ~CBeastEffect() = default;

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
	CTransform* m_pTarget;
	BEASTEFFECTDESC m_Desc;
	bool m_bCreate;
private:
	HRESULT SetUp_Components();

public:
	static CBeastEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
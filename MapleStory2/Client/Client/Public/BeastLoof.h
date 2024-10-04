#pragma once

#include "Creature.h"

BEGIN(Client)

class CBeastLoof final : public CCreature
{
public:
	typedef struct tagBeastLoofDESC
	{
		DIR eDir;
		CTransform* pTransform;
	}BEASTLOOFDESC;

private:
	CBeastLoof(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBeastLoof(const CBeastLoof& rhs);
	virtual ~CBeastLoof() = default;

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
	void SetPosition(DIR eDir);
public:
	virtual void SetAni() override;


private:
	STATE m_eCurState;
	DIR m_eDir;
	CTransform* m_pTarget;
	BEASTLOOFDESC m_Desc;
	bool m_bCreate;
	_float m_fCreateAcc;
private:
	HRESULT SetUp_Components();

public:
	static CBeastLoof* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
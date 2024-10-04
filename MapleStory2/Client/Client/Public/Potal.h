#pragma once

#include "Creature.h"

BEGIN(Client)

class CPotal final : public CCreature
{
public:
	typedef struct tagPotalDesc
	{
		_float3 Pos;
		_float3 DestPos;
		LEVEL eDestLevel;

	}POTALDESC;

private:
	CPotal(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPotal(const CPotal& rhs);
	virtual ~CPotal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;


private:
	_float3 m_vDestPos;
	LEVEL	m_eDestLevel;

	_float m_fTimeAcc = 0.f;
	_bool m_bTrigger = false;
	CGameObject* m_pOther = nullptr;

private:
	HRESULT SetUp_Components();

	virtual HRESULT Set_RenderState() override;
	virtual HRESULT Reset_RenderState() override;

public:
	static CPotal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
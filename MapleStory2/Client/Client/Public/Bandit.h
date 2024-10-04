#pragma once

#include "Creature.h"

BEGIN(Client)

class CBandit final : public CCreature
{
public:
	typedef struct tagBanditDesc
	{
		_float3 vPos;
		_float fScale;
		_float fTime;
	}BANDITDESC;


private:
	CBandit(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBandit(const CBandit& rhs);
	virtual ~CBandit() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;


private:
	HRESULT SetUp_Components();

	virtual HRESULT Set_RenderState() override;
	virtual HRESULT Reset_RenderState() override;

private:
	_float m_fTimeAcc = 0.f;
	_float m_fTime = 0.f;
	_float m_fOriScale = 0.f;
	_float m_fCurScale = 0.f;
	_bool m_bSwitch = false;


public:
	static CBandit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
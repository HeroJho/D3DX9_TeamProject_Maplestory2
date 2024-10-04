#pragma once

#include "Creature.h"

BEGIN(Client)

class CShadow final : public CCreature
{
public:
	typedef struct tagShadowDesc
	{
		_float fScale;

	}SHADOWDESC;

private:
	CShadow(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShadow(const CShadow& rhs);
	virtual ~CShadow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

public:
	_float Get_OriScale() { return m_fOriScale; }


private:
	HRESULT SetUp_Components();

	virtual HRESULT Set_RenderState() override;
	virtual HRESULT Reset_RenderState() override;

private:
	_float m_fOriScale  = 0.f;

public:
	static CShadow* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_ComponentPtr(const _tchar* pComponentTag);
	_float Get_CamDistance() const { return m_fCamDistance; }
	_float Get_YDistance() const { return m_fYDistance; }

	_float Get_ColRad() { return m_fColRad; }
	string Get_Tag() { return m_sTag; }

	_bool Get_Dead() { return m_bDead; }
	virtual void Set_Dead() { m_bDead = true; }

	list<class CBoxCollider*>* Get_BoxColComList() { return &m_BoxColliders; }


	// For. PushBoxCollider
	_bool Get_PushedX() { return m_bPushedX; }
	_bool Get_PushedY() { return m_bPushedY; }
	_uint Get_Damage() { return m_iDamage; }
	void Set_PushedX(_bool bPushedX) { m_bPushedX = bPushedX; }
	void Set_PushedY(_bool bPushedY) { m_bPushedY = bPushedY; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Collision(CGameObject* pOther) {};
	virtual void OnLay(_float3 vOutDis) {};
	virtual void OnLookLay(_float3 vOutDis) {};
	virtual void Damaged(CGameObject* pOther) {};


	virtual list<CGameObject*>* GetCubes() { return nullptr; }

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

protected: /* 객체에게 추가된 컴포넌트들을 키로 분류하여 보관한다. */
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

	list<CBoxCollider*> m_BoxColliders;

protected:
	_float				m_fColRad;
	string				m_sTag;
	_float				m_fCamDistance = 0.f;
	_float				m_fYDistance;
	_bool				m_bDead;

	_bool				m_bPushedX = false;
	_bool				m_bPushedY = false;

	_uint				m_iDamage;

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	HRESULT Add_BoxColComponent(_uint iLevelIndex, const _tchar* pPrototypeTag , void* pArg = nullptr);

	
	HRESULT Compute_CamDistance(_float3 vWorldPos);
	void BoxColCom_Tick(class CTransform* pTrans);
	void BoxColCom_Render(class CTransform* pTrans);

public:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
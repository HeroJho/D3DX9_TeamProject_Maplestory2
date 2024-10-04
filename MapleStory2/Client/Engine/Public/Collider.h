#pragma once

#include "Component.h"

/* 화면에 그려져야할 객체들을 그려져야할 순서대로 모아서 보관한다.  */
/* 보관하고 있는 개첻르의 렌더함수를 호출해주낟. */

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public: /* 그려지는 순서에 따른 정의 */
	enum COLLSIONGROUP { COLLSION_PLAYER, COLLSION_MONSTER, COLLSION_NPC, COLLSION_PLAYER_SKILL, COLLSION_MONSTER_SKILL, COLLSION_BLOCK, COLLSION_TRIGGERBLOCK, COLLSION_PUSHBLOCK, COLLSION_DOWNBLOCK, COLLISION_ITEM, COLLSION_UI, COLLSION_POTAL, COLLSION_MODEL, COLLSION_SECTION, COLLSION_END };


private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;



public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
public:
	HRESULT Add_SphereCollsionGroup(COLLSIONGROUP eCollsionGroup, class CGameObject* pGameObject);
	HRESULT Add_BoxCollsionGroup(COLLSIONGROUP eCollsionGroup, class CGameObject* pGameObject);
	HRESULT Add_PushBoxCollsionGroup(COLLSIONGROUP eCollsionGroup, class CGameObject* pGameObject);
	HRESULT Check_SphereCollsion(COLLSIONGROUP eCollsionGroup_L, COLLSIONGROUP eCollsionGroup_R);
	HRESULT Check_BoxCollsion(COLLSIONGROUP eCollsionGroup_L, COLLSIONGROUP eCollsionGroup_R);
	HRESULT Check_PushBoxCollsion(COLLSIONGROUP eCollsionGroup_L, COLLSIONGROUP eCollsionGroup_R);

	HRESULT Check_PushCubeCollsion(COLLSIONGROUP eCollsionGroup_L);

	HRESULT End_Collsion();

	HRESULT Set_SectionCubes();
	void ResetSection();

public:
	_bool Check_Sphere(CGameObject* pObj_L, CGameObject* pObj_R);
	_bool Check_Box(CGameObject* pObj_L, CGameObject* pObj_R, _bool bPush = false);
	_bool Check_Lay(CGameObject* pObj_L, CGameObject* pObj_R, _float3* pOutDis);
	_bool Check_LookLay(CGameObject* pObj_L, CGameObject* pObj_R, _float3* pOutDis);

private:
	list<class CGameObject*>				m_SphereCollisionObjects[COLLSION_END];
	list<class CGameObject*>				m_BoxCollisionObjects[COLLSION_END];
	list<class CGameObject*>				m_PhshBoxCollisionObjects[COLLSION_END];
	typedef list<class CGameObject*>		COLLSIONOBJECTS;

	list<class CGameObject*>		m_Sections;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
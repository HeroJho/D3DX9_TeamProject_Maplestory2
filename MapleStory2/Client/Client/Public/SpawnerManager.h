#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Spawner.h"

BEGIN(Client)

class CSpawnerManager final : CBase
{
	DECLARE_SINGLETON(CSpawnerManager)

public:
	CSpawnerManager();
	virtual ~CSpawnerManager() = default;


public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void Free_Spawner();

public:

	void Add_Spawner(void* pArg);
	// 몬스터를 통해서 스포너의 인덱스를 확인하는 함수
	void Check_MonsterIndex(_int index);
	void Add_SpawnerInfo(CSpawner* Spawner) { m_SpawnerList.push_back(Spawner); }

	void Clear();

public:
	virtual void Free() override;

private:
		//int	monstercount- 스포너의 인덱스를 받아옴
	_uint	m_iIndexNum;
	list<CSpawner*>			m_SpawnerList;

	_bool m_bTest;
};

END
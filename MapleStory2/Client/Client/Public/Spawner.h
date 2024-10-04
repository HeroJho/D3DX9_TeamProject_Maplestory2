#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CSpawner final : public CGameObject
{
public:
	typedef struct tagSpawnerInfo
	{
		_float3			MonsterPos;
		_int			MonsterNum;
		_tchar*			MonsterName;
		_int			SpawnerNum;
		_float			MonsterColRad;
		LEVEL			Level;
		_tchar*			GAS;
		const _tchar*	LayerTag;
	}SPAWNERINFO;
private:
	CSpawner(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpawner(const CSpawner& rhs);
	virtual ~CSpawner() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float4x4				m_ProjMatrix;

private:
	HRESULT SetUp_Components();

public:
	static CSpawner* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

public:
	void Plus_Mushroom() { ++m_iMonsterNow; }
	void Minus_Mushroom() { --m_iMonsterNow; }
	void Set_Index(_int index) { m_iIndexNum = index; }
	_int Get_Index() { return m_iIndexNum; }

private:
	SPAWNERINFO m_SpawnerInfo;
	_uint		m_iMonsterNow;
	_int		m_iIndexNum;
	_float		m_fCount;
};

END
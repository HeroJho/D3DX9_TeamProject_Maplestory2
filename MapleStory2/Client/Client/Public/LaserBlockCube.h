#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Map_Manager.h"

BEGIN(Engine)
class CCollider;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END


BEGIN(Client)

class CLaserBlockCube final : public CGameObject
{

private:
	CLaserBlockCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLaserBlockCube(const CLaserBlockCube& rhs);
	virtual ~CLaserBlockCube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

private:
	_bool temp = false;
	_bool m_bLend = false;
	_float3 m_vOriPos;
	_bool m_bPicked = false;
	_bool m_bDrop = false;
	_float3 m_vDir;

	_int m_iHp = 10;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CLaserBlockCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
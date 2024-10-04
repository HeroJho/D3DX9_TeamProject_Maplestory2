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

class CPushCube final : public CGameObject
{
public:
	//typedef struct tagCubeDesc
	//{
	//	_float3	vPos;
	//	const _tchar* pTextureTag;
	//}CUBEDESC;
private:
	CPushCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPushCube(const CPushCube& rhs);
	virtual ~CPushCube() = default;

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

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CPushCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
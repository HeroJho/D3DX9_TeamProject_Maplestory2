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
class CShader;
END


BEGIN(Client)

class CSeeCube : public CGameObject
{
public:
	typedef struct tagCubeDesc
	{
		_float3	vPos;
		const _tchar* pTextureTag;
	}CUBEDESC;
protected:
	CSeeCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSeeCube(const CSeeCube& rhs);
	virtual ~CSeeCube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

private:
	_float3			m_vTargetPos = _float3(0.f, 0.f, 0.f);
	CMap_Manager::CUBEDATA* m_pData;

	_bool temp = false;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CSeeCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once

#include "Client_Defines.h"
#include "Map_Manager.h"
#include "Cube.h"


BEGIN(Client)

class CTrigger : public CCube
{
public:
	typedef struct tagCubeDesc
	{
		_float3	vPos;
		const _tchar* pTextureTag;
	}CUBEDESC;
private:
	CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;

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

private:
	CMap_Manager::CUBEDATA* m_pData;

	_bool temp = false;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CTrigger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
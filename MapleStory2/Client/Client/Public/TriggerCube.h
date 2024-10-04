#pragma once

#include "Client_Defines.h"
#include "Map_Manager.h"
#include "Cube.h"


BEGIN(Client)

class CTriggerCube : public CCube
{
public:
	typedef struct tagCubeDesc
	{
		_float3	vPos;
		const _tchar* pTextureTag;
	}CUBEDESC;
private:
	CTriggerCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTriggerCube(const CTriggerCube& rhs);
	virtual ~CTriggerCube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

	void Shake(_float fTimeDelta);
	void Down(_float fTimeDelta);

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

private:
	_float3			m_vTargetPos = _float3(0.f, 0.f, 0.f);
	CMap_Manager::CUBEDATA* m_pData;

	_bool temp = false;

	_float m_fTimeAcc = 0.f;
	_bool m_bDown = false;
	_float m_fShakeTimeAcc = 0.f;
	_bool m_bShakeTurn = false;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CTriggerCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
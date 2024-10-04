#pragma once

#include "Client_Defines.h"
#include "GameObject.h"



BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END


BEGIN(Client)

class CModel_MiniBlock final : public CGameObject
{
public:
	enum MINICOLOR { MINI_RED, MINI_YERROW, MINI_ORRANGE, MINI_END };

public:
	typedef struct tagMiniBlockDesc
	{
		MINICOLOR etype;
		_float3 vPos;

	}MINIBLOCKDESC;

private:
	CModel_MiniBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CModel_MiniBlock(const CModel_MiniBlock& rhs);
	virtual ~CModel_MiniBlock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;


	void Go_DestMini();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

	_bool				m_bLend = false;
	_bool				m_bPicked = false;

	MINIBLOCKDESC m_Desc;

private:


private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CModel_MiniBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
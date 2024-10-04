#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBlueMushmomHpbar final : public CGameObject
{
public:
	typedef struct tagUIInfo
	{
		_float					fX, fY, fSizeX, fSizeY;
	}UIINFO;
private:
	CBlueMushmomHpbar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlueMushmomHpbar(const CBlueMushmomHpbar& rhs);
	virtual ~CBlueMushmomHpbar() = default;

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
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float4x4				m_ProjMatrix;
	UIINFO					m_UIInfo;

private:
	HRESULT SetUp_Components();
private:
	void MouseCollision();

public:
	static CBlueMushmomHpbar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
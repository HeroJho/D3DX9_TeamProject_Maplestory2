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

class CTombUI final : public CGameObject
{
public:
	typedef struct tagUIInfo
	{
		_float					fX, fY, fSizeX, fSizeY;
	}UIINFO;

	enum Collision_Type { TYPE_NO, TYPE_ON, TYPE_DOWN, TYPE_UP, TYPE_PRESSING, TYPE_END };
private:
	CTombUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTombUI(const CTombUI& rhs);
	virtual ~CTombUI() = default;

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
	UIINFO					m_OriginInfo;
	RECT					m_RectUI;
	Collision_Type			m_eCollision;

private:
	HRESULT SetUp_Components();
private:
	void MouseCollision(DIMK m_eMouseKey);

public:
	static CTombUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_float m_fTime;
};

END
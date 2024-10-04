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

class CChat final : public CGameObject
{
public:
	typedef struct tagUIInfo
	{
		_float					fX, fY, fSizeX, fSizeY;
	}UIINFO;
private:
	CChat(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChat(const CChat& rhs);
	virtual ~CChat() = default;

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
	static CChat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_bool	m_bChat;
	_float m_fCount;
	_float m_fCountChat;

	_bool m_bCountCheck;
	_bool m_bCountCheck2;

	_bool m_bWhy;

	LPD3DXFONT m_pFontName;
	LPD3DXFONT m_pFont;

	_uint m_iChatProgress;
	_uint m_iRandom;

public:
	void	Bind_Animation();
	
	void	Create_Chat(string Chat);
};

END
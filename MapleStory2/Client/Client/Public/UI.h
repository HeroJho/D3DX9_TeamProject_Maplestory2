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

class CUI : public CGameObject
{
public:
	enum Collision_Type {TYPE_NO, TYPE_ON, TYPE_DOWN, TYPE_UP, TYPE_PRESSING, TYPE_END};
public:
	typedef struct tagUIInfo
	{
		_float					fMoveX, fMoveY, fX, fY, fSizeX, fSizeY;
		_uint iNum;
		_uint iTextNum;
		const _tchar* pTag;
	}UIINFO;
protected:
	CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_TextureNum(_uint i) { m_iTexturenum = i; }
	void Set_UIMovePos(UIINFO tUIInfo) { m_UIInfo.fX = tUIInfo.fX + m_UIInfo.fMoveX, m_UIInfo.fY = tUIInfo.fY + m_UIInfo.fMoveY; }
	void Set_UIPos(UIINFO tUIInfo) { m_UIInfo.fX = tUIInfo.fX, m_UIInfo.fY = tUIInfo.fY; }
	RECT Get_RectUI() { return m_RectUI; }
	Collision_Type Get_CollisionType() { return m_eCollision; }
	const _tchar* Get_Tag() { return m_UIInfo.pTag; }
	_uint Get_TextNum() { return m_iTexturenum; }
	_uint Get_IndexNum() { return m_UIInfo.iNum; }
	void Set_Render(bool bRender) { m_bRender = bRender; }
	bool Get_Render() { return m_bRender; }

	void Start_CutScene(_float fTimeDelta);
	void End_CutScene(_float fTimeDelta);
	

protected:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
protected:
	_float4x4				m_ProjMatrix;
	_float4x4				m_ViewMatrix;
	UIINFO					m_UIInfo;
	UIINFO					m_OriginInfo;
	RECT					m_RectUI;
	Collision_Type			m_eCollision;
	_int					m_iTexturenum;
	_float3					m_vOriginPos;

protected:
	HRESULT SetUp_Components();
	virtual HRESULT Set_RenderState();
	virtual HRESULT Reset_RenderState();
	void Check_Collision(DIMK m_eMouseKey);

protected:
	_bool m_bRender;

public:
	static CUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
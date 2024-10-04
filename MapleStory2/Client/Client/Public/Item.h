#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "InvenManager.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)
class CItem : public CGameObject
{
public:
	typedef struct tagItemInfo
	{
		_float3	vPos;
		_uint iTextNum;
		CInvenManager::InvenType eType;
		const _tchar* pTag;
	}ITEMINFO;
public:
	CItem(LPDIRECT3DDEVICE9 pGraphicDevice);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	virtual void Collision(CGameObject* pOther) override;
	void Set_Billboard();
	void MoveItem(_float fTimeDelta);
	void GetItem(_float fTimeDelta);

protected:
	HRESULT SetUp_Components();
	virtual HRESULT Set_RenderState();
	virtual HRESULT Reset_RenderState();

protected:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;

private:
	ITEMINFO m_ItemInfo;
	_float3 m_vLookTemp;
	_uint m_iDirection;
	bool m_bUP;

	_float3 m_vLook;
	_bool m_bDrop = false;
	_float m_fDropY = 0.f;
	_bool m_bDance = false;

	_float m_fGetItemAcc = 0.f;
	_bool m_bGetItem = false;
	
	_float m_fTimeAcc = 0.f;


public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

#pragma once
#include "UI.h"
#include "InvenManager.h"
#include "GameObject.h"
BEGIN(Engine)
class CTexture;
END
BEGIN(Client)


class CMouseItemIcon final : public  CUI
{
private:
	CMouseItemIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMouseItemIcon(const CMouseItemIcon& rhs);
	virtual ~CMouseItemIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();
	_uint Get_TextNum() { return m_iTexturenum; }
	void Set_Type(CInvenManager::InvenType eType) { m_eType = eType; }
	
public:
	static CMouseItemIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	CInvenManager::InvenType m_eType;
	CTexture* m_pTextureCom2 = nullptr;
};
END


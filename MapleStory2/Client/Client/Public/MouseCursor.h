#pragma once
#include "UI.h"

BEGIN(Client)
class CMouseCursor final : public  CUI
{
private:
	CMouseCursor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMouseCursor(const CMouseCursor& rhs);
	virtual ~CMouseCursor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();
	virtual HRESULT Set_RenderState();
	virtual HRESULT Reset_RenderState();

public:
	static CMouseCursor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


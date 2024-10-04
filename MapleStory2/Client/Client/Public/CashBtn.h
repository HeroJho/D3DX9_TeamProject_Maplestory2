#pragma once
#include "UI.h"

BEGIN(Client)
class CCashBtn final : public  CUI
{
private:
	CCashBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCashBtn(const CCashBtn& rhs);
	virtual ~CCashBtn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CCashBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


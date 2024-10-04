#pragma once
#include "UI.h"

BEGIN(Client)
class CStuffBtn final : public  CUI
{
private:
	CStuffBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStuffBtn(const CStuffBtn& rhs);
	virtual ~CStuffBtn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CStuffBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


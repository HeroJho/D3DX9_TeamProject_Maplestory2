#pragma once
#include "UI.h"

BEGIN(Client)
class CInstallBtn final : public  CUI
{
private:
	CInstallBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInstallBtn(const CInstallBtn& rhs);
	virtual ~CInstallBtn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CInstallBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


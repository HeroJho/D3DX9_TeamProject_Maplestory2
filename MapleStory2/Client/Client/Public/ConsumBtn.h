#pragma once
#include "UI.h"

BEGIN(Client)
class CConsumBtn final : public  CUI
{
private:
	CConsumBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CConsumBtn(const CConsumBtn& rhs);
	virtual ~CConsumBtn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CConsumBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


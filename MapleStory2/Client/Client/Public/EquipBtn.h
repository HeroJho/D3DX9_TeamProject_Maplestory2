#pragma once
#include "UI.h"

BEGIN(Client)
class CEquipBtn final : public  CUI
{
private:
	CEquipBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEquipBtn(const CEquipBtn& rhs);
	virtual ~CEquipBtn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CEquipBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


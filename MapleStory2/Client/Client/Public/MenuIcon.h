#pragma once
#include "UI.h"

BEGIN(Client)
class CMenuIcon final : public  CUI
{
private:
	CMenuIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMenuIcon(const CMenuIcon& rhs);
	virtual ~CMenuIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CMenuIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END


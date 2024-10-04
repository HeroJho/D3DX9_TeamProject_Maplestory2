#pragma once
#include "UI.h"
#include "ReefAttackInfo.h"

BEGIN(Client)
class CBlackWizardScene final : public  CUI
{
private:
	CBlackWizardScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizardScene(const CBlackWizardScene& rhs);
	virtual ~CBlackWizardScene() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float m_fAniAcc;
	bool m_bFinal;
public:
	static CBlackWizardScene* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END


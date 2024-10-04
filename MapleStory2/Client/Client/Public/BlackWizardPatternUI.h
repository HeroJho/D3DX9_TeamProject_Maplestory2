#pragma once
#include "UI.h"
#include "ReefAttackInfo.h"

BEGIN(Client)
class CBlackWizardPatternUI final : public  CUI
{
private:
	CBlackWizardPatternUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlackWizardPatternUI(const CBlackWizardPatternUI& rhs);
	virtual ~CBlackWizardPatternUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Shake();
	void Set_TextNum(_int iNum);
private:
	bool m_bShake;
	bool m_bRight;
	_float m_fShakeAcc;
	_float3 m_vOriginPos;
public:
	static CBlackWizardPatternUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};
END


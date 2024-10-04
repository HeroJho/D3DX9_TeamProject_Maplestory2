#pragma once
#include "UI.h"
#include "SpearPullingInfo.h"

BEGIN(Client)
class CSpearPullingIcon final : public  CUI
{
private:
	CSpearPullingIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpearPullingIcon(const CSpearPullingIcon& rhs);
	virtual ~CSpearPullingIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CSpearPullingIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iSkillPoint;
	CSpearPullingInfo* m_pSkillInfo;
	LPD3DXFONT m_NoticeFont;
};
END


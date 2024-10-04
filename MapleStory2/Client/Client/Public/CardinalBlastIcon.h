#pragma once
#include "UI.h"
#include "ReefAttackInfo.h"

BEGIN(Client)
class CCardinalBlastIcon final : public  CUI
{
private:
	CCardinalBlastIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCardinalBlastIcon(const CCardinalBlastIcon& rhs);
	virtual ~CCardinalBlastIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CCardinalBlastIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iSkillPoint;
	CReefAttackInfo* m_pSkillInfo;
	LPD3DXFONT m_NoticeFont;
};
END


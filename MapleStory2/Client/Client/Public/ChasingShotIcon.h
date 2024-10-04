#pragma once
#include "UI.h"
#include "ReefAttackInfo.h"

BEGIN(Client)
class CChasingShotIcon final : public  CUI
{
private:
	CChasingShotIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CChasingShotIcon(const CChasingShotIcon& rhs);
	virtual ~CChasingShotIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CChasingShotIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iSkillPoint;
	CReefAttackInfo* m_pSkillInfo;
	LPD3DXFONT m_NoticeFont;
};
END


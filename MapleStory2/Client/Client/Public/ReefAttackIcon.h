#pragma once
#include "UI.h"
#include "ReefAttackInfo.h"

BEGIN(Client)
class CReefAttackIcon final : public  CUI
{
private:
	CReefAttackIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CReefAttackIcon(const CReefAttackIcon& rhs);
	virtual ~CReefAttackIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CReefAttackIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iSkillPoint;
	CReefAttackInfo* m_pSkillInfo;
	LPD3DXFONT m_NoticeFont;
};
END


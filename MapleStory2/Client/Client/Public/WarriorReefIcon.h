#pragma once
#include "UI.h"
#include "WarriorReefInfo.h"

BEGIN(Client)
class CWarriorReefIcon final : public  CUI
{
private:
	CWarriorReefIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWarriorReefIcon(const CWarriorReefIcon& rhs);
	virtual ~CWarriorReefIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CWarriorReefIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iSkillPoint;
	CWarriorReefInfo* m_pSkillInfo;
	LPD3DXFONT m_NoticeFont;
};
END


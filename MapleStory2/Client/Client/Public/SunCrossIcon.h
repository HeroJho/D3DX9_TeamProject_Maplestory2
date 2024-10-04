#pragma once
#include "UI.h"
#include "SunCrossInfo.h"

BEGIN(Client)
class CSunCrossIcon final : public  CUI
{
private:
	CSunCrossIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSunCrossIcon(const CSunCrossIcon& rhs);
	virtual ~CSunCrossIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CSunCrossIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iSkillPoint;
	CSunCrossInfo* m_pSkillInfo;
	LPD3DXFONT m_NoticeFont;
};
END


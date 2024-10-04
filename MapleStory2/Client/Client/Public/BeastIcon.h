#pragma once
#include "UI.h"
#include "SolunaSlashInfo.h"

BEGIN(Client)
class CBeastIcon final : public  CUI
{
private:
	CBeastIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBeastIcon(const CBeastIcon& rhs);
	virtual ~CBeastIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CBeastIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	CSolunaSlashInfo* m_pSkillInfo;
	_uint m_iSkillPoint;
	LPD3DXFONT m_NoticeFont;
};
END


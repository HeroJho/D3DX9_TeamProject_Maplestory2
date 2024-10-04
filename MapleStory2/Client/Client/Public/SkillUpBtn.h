#pragma once
#include "UI.h"

BEGIN(Client)
class CSkillUpBtn final : public  CUI
{
private:
	CSkillUpBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkillUpBtn(const CSkillUpBtn& rhs);
	virtual ~CSkillUpBtn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();
	void Set_RenderGroup();
	void SkillLevelUp();
	void SkillLevelUp_Begenner();
	void SkillLevelUp_First();
	void SkillLevelUp_Second();
	void SkillLevelUp_Third();
	void SkillLevelUp_Fourth();
	void SkillLevelUp_Fifth();

public:
	static CSkillUpBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	_uint m_iSkillPoint;
	_uint m_iBtnNum;
};
END


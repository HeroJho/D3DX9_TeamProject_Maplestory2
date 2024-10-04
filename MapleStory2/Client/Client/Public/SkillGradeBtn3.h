#pragma once
#include "UI.h"

BEGIN(Client)
class CSkillGradeBtn3 final : public  CUI
{
private:
	CSkillGradeBtn3(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkillGradeBtn3(const CSkillGradeBtn3& rhs);
	virtual ~CSkillGradeBtn3() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CSkillGradeBtn3* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


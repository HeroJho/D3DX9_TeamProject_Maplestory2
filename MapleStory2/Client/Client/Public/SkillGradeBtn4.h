#pragma once
#include "UI.h"

BEGIN(Client)
class CSkillGradeBtn4 final : public  CUI
{
private:
	CSkillGradeBtn4(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkillGradeBtn4(const CSkillGradeBtn4& rhs);
	virtual ~CSkillGradeBtn4() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CSkillGradeBtn4* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


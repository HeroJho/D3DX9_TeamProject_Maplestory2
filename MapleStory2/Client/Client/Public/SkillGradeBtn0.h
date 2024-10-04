#pragma once
#include "UI.h"

BEGIN(Client)
class CSkillGradeBtn0 final : public  CUI
{
private:
	CSkillGradeBtn0(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkillGradeBtn0(const CSkillGradeBtn0& rhs);
	virtual ~CSkillGradeBtn0() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CSkillGradeBtn0* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


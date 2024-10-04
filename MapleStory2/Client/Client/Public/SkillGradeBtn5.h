#pragma once
#include "UI.h"

BEGIN(Client)
class CSkillGradeBtn5 final : public  CUI
{
private:
	CSkillGradeBtn5(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkillGradeBtn5(const CSkillGradeBtn5& rhs);
	virtual ~CSkillGradeBtn5() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();

public:
	static CSkillGradeBtn5* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


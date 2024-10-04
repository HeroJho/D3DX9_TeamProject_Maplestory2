#pragma once
#include "Client_Defines.h"
#include "SkillManager.h"
#include "SkillInfo.h"
#include "UI.h"

BEGIN(Client)

class CSkillFrame final : public CUI
{
private:
	CSkillFrame(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkillFrame(const CSkillFrame& rhs);
	virtual ~CSkillFrame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Add_SkillFrameImage(CUI* pSkillFrameImage);

public:
	static CSkillFrame* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

private:
	void RenderText();
	void BegennerText();
	void FirstText();
	void SecondText();
	void ThirdText();
	void FourthText();
	void FifthText();
	void Set_Data();
private:
	_uint m_iSkillPointDigit;
	LPD3DXFONT m_FrameFont;
	LPD3DXFONT m_GradeFont;
	const wchar_t* m_cGrade;

private:
	CSkillManager* m_pSkillManager;
	list<CUI*> m_SkillFrameImage;

};

END


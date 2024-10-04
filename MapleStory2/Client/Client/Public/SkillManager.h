#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CSkillFrame;
class CUI;
class CSkillInfo;

class CSkillManager final : CBase
{
	DECLARE_SINGLETON(CSkillManager)
public:
	enum SkillGrade {GRADE_BEGENNER, GRADE_FIRST, GRADE_SECOND, GRADE_THIRD, GRADE_FOURTH, GRADE_FIFTH, GRADE_END};
public:
	CSkillManager();
	virtual ~CSkillManager() = default;

public:
	const _uint Get_SkillPoint() { return m_iSkillPoint; }
	void Set_SkillPoint(_uint iSkillPoint) { m_iSkillPoint += iSkillPoint; }
	
	HRESULT Set_pSkillFrame(CSkillFrame* pSkillFrame);
	HRESULT Add_SkillFrameImage(CUI* pSkillFrameImage);
	HRESULT Add_SkillInfo(const _tchar* pTag, SkillGrade eGrade, CSkillInfo* pSkillInfo);

	const SkillGrade Get_SkillGrade() { return m_eSkillGrade; }
	void Set_SkillGrade(SkillGrade eGrade) { m_eSkillGrade = eGrade; }
	_uint Get_PlayerGrade() { return m_iGrade; }
	void Set_PlayerGrade(_uint iGrade) { m_iGrade = iGrade; }
	CSkillInfo* Get_SkillInfo(const _tchar* pSkillInfoTag, SkillGrade eGrade);
	CSkillInfo* Find_SkillInfo(const _tchar* pSkillInfoTag, SkillGrade eGrade);
	
private:
	_uint m_iSkillPoint;
	_uint m_iGrade;
	CSkillFrame* m_pSkillFrame;
	SkillGrade m_eSkillGrade;
	map<const _tchar*, CSkillInfo*> m_SkillInfo[GRADE_FIFTH];
public:
	virtual void Free() override;

};

END
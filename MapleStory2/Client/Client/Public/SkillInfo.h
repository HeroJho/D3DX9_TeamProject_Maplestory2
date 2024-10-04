#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Client)
class CSkillInfo : public CBase
{
protected:
	CSkillInfo();
	virtual ~CSkillInfo() = default;

public:
	const _uint Get_SkillLevel() { return m_iSkillLevel; }
	void Set_SkillLevel(_uint iSkillLevel) { m_iSkillLevel += iSkillLevel; }
	const _uint Get_SkillDamage() { return m_iSkillDamage; }
	void Set_SkillDamage(_uint iSkillDamage) { m_iSkillDamage += iSkillDamage; }
	const wchar_t* Get_SkillName() { return m_cSkillName; }
	const wchar_t* Get_SkillNotice() { return m_cSkillNotice; }
	void Set_SkillNotice(const wchar_t* pSkillNotice) { m_cSkillNotice = pSkillNotice; }
	const _uint Get_TextNum() { return m_iTextNum; }

public:
	virtual void Free() override;

protected:
	const wchar_t* m_cSkillName;
	const wchar_t* m_cSkillNotice;
	_uint m_iSkillDamage;
	_uint m_iSkillLevel;
	_uint m_iTextNum;
};
END


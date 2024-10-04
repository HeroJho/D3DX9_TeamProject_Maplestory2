#include "stdafx.h"
#include "..\Public\ReefAttackInfo.h"


CReefAttackInfo::CReefAttackInfo()
{
	Initialize();
}

void CReefAttackInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0;
	m_iTextNum = 2;
	m_cSkillName = L"리프 어택";
	m_cSkillNotice = L"타입 : 전방 공격\n대미지 : 10\n타격횟수 : 2\n소모 마나 : 30";
}

void CReefAttackInfo::Free()
{
	__super::Free();
}


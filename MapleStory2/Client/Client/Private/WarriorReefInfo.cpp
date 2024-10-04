#include "stdafx.h"
#include "..\Public\WarriorReefInfo.h"


CWarriorReefInfo::CWarriorReefInfo()
{
	Initialize();
}

void CWarriorReefInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0;
	m_iTextNum = 4;
	m_cSkillName = L"워리어 리프";
	m_cSkillNotice = L"\n타입 : 대쉬\n 소모 마나 : 10\n이동 거리 : 200px";
}

void CWarriorReefInfo::Free()
{
	__super::Free();
}


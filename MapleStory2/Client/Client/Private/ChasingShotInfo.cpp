#include "stdafx.h"
#include "..\Public\ChasingShotInfo.h"


CChasingShotInfo::CChasingShotInfo()
{
	Initialize();
}

void CChasingShotInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0;
	m_iTextNum = 6;
	m_cSkillName = L"체이싱 샷";
	m_cSkillNotice = L"타입 : 원거리 공격\n대미지 : 100\n타격횟수 : 1\n소모 마나 : 200";
}

void CChasingShotInfo::Free()
{
	__super::Free();
}


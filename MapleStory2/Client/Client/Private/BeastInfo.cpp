#include "stdafx.h"
#include "..\Public\BeastInfo.h"


CBeastInfo::CBeastInfo()
{
	Initialize();
}

void CBeastInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0;
	m_iTextNum = 7;
	m_cSkillName = L"헬비스트";
	m_cSkillNotice = L"타입 : 광역 공격\n대미지 : 500\n타격횟수 : 1\n소모 마나 : 300";
}

void CBeastInfo::Free()
{
	__super::Free();
}


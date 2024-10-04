#include "stdafx.h"
#include "..\Public\SunCrossInfo.h"


CSunCrossInfo::CSunCrossInfo()
{
	Initialize();
}

void CSunCrossInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0; 
	m_iTextNum = 0;
	m_cSkillName = L"썬 크로스";
	m_cSkillNotice = L"타입 : 전방 공격\n대미지 : 20\n타격횟수 : 3\n소모 마나 : 50";
	int a = m_dwRefCnt;
}

void CSunCrossInfo::Free()
{
	__super::Free();
}


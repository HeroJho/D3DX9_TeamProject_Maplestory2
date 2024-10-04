#include "stdafx.h"
#include "..\Public\SpearPullingInfo.h"


CSpearPullingInfo::CSpearPullingInfo()
{
	Initialize();
}

void CSpearPullingInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0;
	m_iTextNum = 3;
	m_cSkillName = L"스피어 풀링";
	m_cSkillNotice = L"타입 : 몹몰이\n대미지 : 30\n타격횟수 : 5\n소모 마나 : 70";
}

void CSpearPullingInfo::Free()
{
	__super::Free();
}


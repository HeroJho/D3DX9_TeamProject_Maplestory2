#include "stdafx.h"
#include "..\Public\CardinalBlastInfo.h"


CCardinalBlastInfo::CCardinalBlastInfo()
{
	Initialize();
}

void CCardinalBlastInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0;
	m_iTextNum = 5;
	m_cSkillName = L"블래스트 샷";
	m_cSkillNotice = L"타입 : 원거리 공격\n대미지 : 20\n타격횟수 : 1\n소모 마나 : 50";
}

void CCardinalBlastInfo::Free()
{
	__super::Free();
}


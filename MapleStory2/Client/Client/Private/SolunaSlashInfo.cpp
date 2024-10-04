#include "stdafx.h"
#include "..\Public\SolunaSlashInfo.h"


CSolunaSlashInfo::CSolunaSlashInfo()
{
	Initialize();
}

void CSolunaSlashInfo::Initialize()
{
	m_iSkillLevel = 0;
	m_iSkillDamage = 0;
	m_iTextNum = 1;
	m_cSkillName = L"솔루나 슬래쉬";
	m_cSkillNotice = L"스킬 설명 테스트\n타입 : 대쉬\n 소모 마나 : 30\n이동 거리 : 300px";
}

void CSolunaSlashInfo::Free()
{
	__super::Free();
}


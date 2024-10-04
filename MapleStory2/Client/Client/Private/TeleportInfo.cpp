#include "stdafx.h"
#include "..\Public\TeleportInfo.h"


CTeleportInfo::CTeleportInfo()
{
	Initialize();
}

void CTeleportInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 999;
	m_iTextNum = 2;
	m_pItemName = L"TeleportInfo";
	m_pItemNotice = L"마을로 귀환합니다";
}

void CTeleportInfo::Free()
{
}



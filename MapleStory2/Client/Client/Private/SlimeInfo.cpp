#include "stdafx.h"
#include "..\Public\SlimeInfo.h"


CSlimeInfo::CSlimeInfo()
{
	Initialize();
}

void CSlimeInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 99;
	m_iTextNum = 2;
	m_pItemName = L"SlimeInfo";
	m_pItemNotice = L"슬라임액체";
}

void CSlimeInfo::Free()
{
}



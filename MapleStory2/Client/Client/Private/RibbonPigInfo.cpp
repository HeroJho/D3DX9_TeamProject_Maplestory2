#include "stdafx.h"
#include "..\Public\RibbonPigInfo.h"


CRibbonPigInfo::CRibbonPigInfo()
{
	Initialize();
}

void CRibbonPigInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 99;
	m_iTextNum = 1;
	m_pItemName = L"RibbonPigInfo";
	m_pItemNotice = L"¸®º»";
}

void CRibbonPigInfo::Free()
{
}



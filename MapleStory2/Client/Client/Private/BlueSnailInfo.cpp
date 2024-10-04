#include "stdafx.h"
#include "..\Public\BlueSnailInfo.h"


CBlueSnailInfo::CBlueSnailInfo()
{
	Initialize();
}

void CBlueSnailInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 99;
	m_iTextNum = 0;
	m_pItemName = L"BlueSnailInfo";
	m_pItemNotice = L"ÆÄ¶õ´ÞÆØÀÌ²®Áú";
}

void CBlueSnailInfo::Free()
{
}



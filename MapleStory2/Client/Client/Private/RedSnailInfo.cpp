#include "stdafx.h"
#include "..\Public\RedSnailInfo.h"


CRedSnailInfo::CRedSnailInfo()
{
	Initialize();
}

void CRedSnailInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 99;
	m_iTextNum = 5;
	m_pItemName = L"RedSnailInfo";
	m_pItemNotice = L"ª°∞£¥ﬁ∆ÿ¿Ã≤Æ¡˙";
}

void CRedSnailInfo::Free()
{
}



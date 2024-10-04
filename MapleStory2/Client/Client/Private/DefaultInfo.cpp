#include "stdafx.h"
#include "..\Public\DefaultInfo.h"


CDefaultInfo::CDefaultInfo()
{
	Initialize();
}

void CDefaultInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 999;
	m_iTextNum = 99;
	m_pItemName = L"DefaultInfo";
	m_pItemNotice = L"";
}

void CDefaultInfo::Free()
{
}



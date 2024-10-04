#include "stdafx.h"
#include "..\Public\GoStumpInfo.h"


CGoStumpInfo::CGoStumpInfo()
{
	Initialize();
}

void CGoStumpInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 99;
	m_iTextNum = 4;
	m_pItemName = L"GoStumpInfo";
	m_pItemNotice = L"¿Â¿€";
}

void CGoStumpInfo::Free()
{
}



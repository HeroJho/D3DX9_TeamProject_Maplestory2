#include "stdafx.h"
#include "..\Public\GreenMushroomInfo.h"


CGreenMushroomInfo::CGreenMushroomInfo()
{
	Initialize();
}

void CGreenMushroomInfo::Initialize()
{
	m_iNowNum = 0;
	m_iMaxNum = 99;
	m_iTextNum = 3;
	m_pItemName = L"GreenMushroomInfo";
	m_pItemNotice = L"ÃÊ·Ô¹ö¼¸ÀÇ°«";
}

void CGreenMushroomInfo::Free()
{
}



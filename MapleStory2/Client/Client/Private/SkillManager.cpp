#include "stdafx.h"
#include "..\Public\SkillManager.h"
#include "SkillFrame.h"
#include "UI.h"
#include "SkillInfo.h"

IMPLEMENT_SINGLETON(CSkillManager)

CSkillManager::CSkillManager()
	: m_iSkillPoint(0),m_eSkillGrade(GRADE_BEGENNER)
{
}

HRESULT CSkillManager::Set_pSkillFrame(CSkillFrame* pSkillFrame)
{
	if (pSkillFrame == nullptr)
		return E_FAIL;

	m_pSkillFrame = pSkillFrame;
	m_iGrade = 0;
	Safe_AddRef(m_pSkillFrame);

	return S_OK;
}

HRESULT CSkillManager::Add_SkillFrameImage(CUI* pSkillFrameImage)
{
	if (pSkillFrameImage == nullptr)
		return E_FAIL;

	m_pSkillFrame->Add_SkillFrameImage(pSkillFrameImage);
	//Safe_AddRef(pSkillFrameImage);

	return S_OK;
}

HRESULT CSkillManager::Add_SkillInfo(const _tchar* pTag, SkillGrade eGrade, CSkillInfo * pSkillInfo)
{
	if (pSkillInfo == nullptr)
		return E_FAIL;

	m_SkillInfo[eGrade].emplace(pTag, pSkillInfo);
	
	
	return S_OK;
}

CSkillInfo* CSkillManager::Get_SkillInfo(const _tchar * pSkillInfoTag, SkillGrade eGrade)
{
	return 	Find_SkillInfo(pSkillInfoTag, eGrade);
}

CSkillInfo* CSkillManager::Find_SkillInfo(const _tchar * pSkillInfoTag, SkillGrade eGrade)
{
	auto	iter = find_if(m_SkillInfo[eGrade].begin(), m_SkillInfo[eGrade].end(), CTag_Finder(pSkillInfoTag));

	if (iter == m_SkillInfo[eGrade].end())
		return nullptr;

	return iter->second;
}


void CSkillManager::Free()
{
	for (size_t i = GRADE_BEGENNER; i < GRADE_END - 1; ++i)
	{
		for (auto& pair : m_SkillInfo[i])
		{
			Safe_Release(pair.second);
		}
	}
	m_SkillInfo->clear();

	Safe_Release(m_pSkillFrame);
}



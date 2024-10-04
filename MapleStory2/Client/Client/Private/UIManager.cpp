#include "stdafx.h"
#include "..\Public\UIManager.h"
#include "GameInstance.h"
#include "Loading.h"
#include "CutScreen.h"
#include "DomiScreen.h"
#include "TaxiChatBox.h"
#include "EHelenaChat.h"
#include "Title.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
{
	m_iBlueMushmomHp = 20;
	m_iGASHp = 50;
	m_bStartMove = false;
	m_bEndMove = false;
	m_iPlayerHp = 0;
	m_iPlayerMp = 0;
	m_iPlayerLevel = 1;
	m_iPlayerExp = 0;
}

HRESULT CUIManager::Add_ItemNotice(CItemNotice * pItemNotice)
{
	if (pItemNotice == nullptr)
		return E_FAIL;
	Safe_AddRef(pItemNotice);
	m_pItemNotice = pItemNotice;

	return S_OK;
}

void CUIManager::Set_ItemNoticeTextNum(_uint iTextNum, bool bRender)
{
	m_pItemNotice->Set_TextureNum(iTextNum);
	m_pItemNotice->Set_Render(bRender);	
}

bool CUIManager::Check_Change(_uint iTextNum)
{
	if (m_pItemNotice->Get_Render() && m_pItemNotice->Get_TextNum() == iTextNum)
		return true;

	return false;
}
void CUIManager::Set_Loading()
{
	if (nullptr != m_pLoading)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CGameObject* pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Loading"), &pObj, nullptr)))
		return;
	m_pLoading = (CLoading*)pObj;

	pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CutScreen"), &pObj, nullptr)))
		return;
	m_pCutScreen = (CCutScreen*)pObj;

	pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_DomiScreen"), &pObj, nullptr)))
		return;
	m_pDomiScreen = (CDomiScreen*)pObj;

	pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Title"), &pObj, nullptr)))
		return;
	m_pTitle = (CTitle*)pObj;


	Safe_Release(pGameInstance);
}

void CUIManager::Start_Loading()
{
	if (nullptr == m_pLoading)
		return;

	m_pLoading->StartLoading();
}
void CUIManager::End_Loading()
{
	if (nullptr == m_pLoading)
		return;

	m_pLoading->EndLoading();
}

void CUIManager::On_CutScreen()
{
	if (nullptr == m_pCutScreen)
		return;

	m_pCutScreen->OnCutScreen();
}
void CUIManager::Off_CutScreen()
{
	if (nullptr == m_pCutScreen)
		return;

	m_pCutScreen->OffCutScreen();
}

void CUIManager::On_DomiScreen()
{
	if (nullptr == m_pDomiScreen)
		return;

	m_pDomiScreen->OnCutScreen();
}
void CUIManager::Off_DomiScreen()
{
	if (nullptr == m_pDomiScreen)
		return;

	m_pDomiScreen->OffCutScreen();
}

void CUIManager::On_TaxiChatBox()
{
	if (nullptr == m_pTaxiChatBox)
		return;

	m_pTaxiChatBox->Start_Chat();
}

void CUIManager::Off_TaxiChatBox()
{
	if (nullptr == m_pTaxiChatBox)
		return;

	m_pTaxiChatBox->End_Chat();
}

void CUIManager::On_EHelenaChat()
{
	if (nullptr == m_pEHelenaChat)
		return;

	m_pEHelenaChat->Start_Chat();
}

void CUIManager::Off_EHelenaChat()
{
	if (nullptr == m_pEHelenaChat)
		return;

	m_pEHelenaChat->End_Chat();
}

void CUIManager::PlayTitleCurMap(LEVEL eLevel)
{
	if (nullptr == m_pTitle)
		return;

	m_pTitle->PlayTitleCurMap(eLevel);
}

void CUIManager::Tick(_float TimeDelta)
{
	m_pTitle->Tick(TimeDelta);
	m_pLoading->Tick(TimeDelta);
	m_pCutScreen->Tick(TimeDelta);
	m_pDomiScreen->Tick(TimeDelta);
}

void CUIManager::LateTick(_float TimeDelta)
{
	m_pTitle->LateTick(TimeDelta);
	m_pLoading->LateTick(TimeDelta);
	m_pCutScreen->LateTick(TimeDelta);
	m_pDomiScreen->LateTick(TimeDelta);
}

void CUIManager::Fix_BlackWizardHp(_int iHp)
{
	m_iBlackWizardHp += iHp;
	if (m_iBlackWizardHp >= m_iBlackWizardMaxHp)
		m_iBlackWizardHp = m_iBlackWizardMaxHp;
	else if (m_iBlackWizardHp <= 0)
		m_iBlackWizardHp = 0;
}

HRESULT CUIManager::Add_BlackWizardPatternUI(CBlackWizardPatternUI * pInstance)
{
	if (pInstance == nullptr)
		return E_FAIL;
	m_pBlackWizardPatternUI = pInstance;


	return S_OK;
}

void CUIManager::Set_PlayerHp(_int iHp)
{
	if (m_iPlayerHp + iHp >= 21)
		m_iPlayerHp = 21;
	else
		m_iPlayerHp += iHp;
}

void CUIManager::Set_PlayerMp(_int iMp)
{
	if (m_iPlayerMp + iMp >= 22)
		m_iPlayerMp = 22;
	else
		m_iPlayerMp += iMp;
}

void CUIManager::Level_Up(_uint iLevel)
{
	CUIManager::Get_Instance()->Full_PlayerHp();
	CUIManager::Get_Instance()->Full_PlayerMp();
	m_iPlayerLevel += iLevel;
}

void CUIManager::Free()
{
	Safe_Release(m_pTitle);
	Safe_Release(m_pDomiScreen);
	Safe_Release(m_pItemNotice);
	Safe_Release(m_pCutScreen);
	Safe_Release(m_pLoading);
}



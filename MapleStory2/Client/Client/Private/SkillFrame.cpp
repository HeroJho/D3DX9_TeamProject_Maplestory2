#include "stdafx.h"
#include "..\Public\SkillFrame.h"
#include "GameInstance.h"
#include "UIManager.h"


CSkillFrame::CSkillFrame(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CSkillFrame::CSkillFrame(const CSkillFrame& rhs)
	:CUI(rhs)
{
}


HRESULT CSkillFrame::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CSkillFrame::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));

	m_pSkillManager = CSkillManager::Get_Instance();

	m_pSkillManager->Set_pSkillFrame(this);

	__super::Initialize(pArg);
	
	D3DXCreateFont(m_pGraphic_Device, 13, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"돋움체", &m_FrameFont);

	D3DXCreateFont(m_pGraphic_Device, 18, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"돋움체", &m_GradeFont);


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkillFrame"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	m_bRender = false;

	return S_OK;
}

void CSkillFrame::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	Check_Collision(DIMK_LBUTTON);
	if (pInstance->Key_Down(DIK_K))
	{
		if (m_bRender)
			CGameInstance::Get_Instance()->PlaySoundW(L"MenuDown.mp3", 25, 1.f);
		else
			CGameInstance::Get_Instance()->PlaySoundW(L"MenuUp.mp3", 25, 1.f);
		m_bRender = !m_bRender;
	}
	if (pInstance->Key_Down(DIK_P))
	{
		m_pSkillManager->Set_SkillPoint(1);
		
	}

	switch (CUIManager::Get_Instance()->Get_PlayerLevel() / 10)
	{
	case 0:
		m_pSkillManager->Set_PlayerGrade(0);
		break;
	case 1:
		m_pSkillManager->Set_PlayerGrade(1);
		break;
	case 2:
		m_pSkillManager->Set_PlayerGrade(2);
		break;
	case 3:
		m_pSkillManager->Set_PlayerGrade(3);
		break;
	case 4:
		m_pSkillManager->Set_PlayerGrade(4);
		break;
	default:
		break;
	}
		
	/*if (pInstance->Mouse_Pressing(DIMK_LBUTTON))
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
	
		if (PtInRect(&m_RectUI, ptMouse))
		{
			m_UIInfo.fX = ptMouse.x;
			m_UIInfo.fY = ptMouse.y+150;
		}

	}*/

	Safe_Release(pInstance);
}

void CSkillFrame::LateTick(_float fTimeDelta)
{
	
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	for (auto list : m_SkillFrameImage)
	{
		list->Set_UIMovePos(m_UIInfo);
	}
	Set_Data();


	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	switch (pSkillInstance->Get_SkillGrade())
	{
	case CSkillManager::GRADE_BEGENNER:
		m_iTexturenum = 1;
		break;
	case CSkillManager::GRADE_FIRST:
		m_iTexturenum = 1;
		break;
	case CSkillManager::GRADE_SECOND:
		m_iTexturenum = 1;
		break;
	case CSkillManager::GRADE_THIRD:
		m_iTexturenum = 2;
		break;
	case CSkillManager::GRADE_FOURTH:
		m_iTexturenum = 2;
		break;
	case CSkillManager::GRADE_FIFTH:
		m_iTexturenum = 2;
		break;

	default:
		break;
	}
	//SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));


	

}

HRESULT CSkillFrame::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();
	
	RenderText();
	
	return S_OK;
}

HRESULT CSkillFrame::Add_SkillFrameImage(CUI * pSkillFrameImage)
{
	if (pSkillFrameImage == nullptr)
		return E_FAIL;

	m_SkillFrameImage.push_back(pSkillFrameImage);
	return S_OK;
}




CSkillFrame* CSkillFrame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkillFrame*		pInstance = new CSkillFrame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSkillFrame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillFrame::Clone(void * pArg)
{
	CSkillFrame*		pInstance = new CSkillFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSkillFrame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkillFrame::RenderText()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	switch (pSkillInstance->Get_SkillGrade())
	{
	case CSkillManager::GRADE_BEGENNER:
		BegennerText();
		break;
	case CSkillManager::GRADE_FIRST:
		FirstText();
		break;
	case CSkillManager::GRADE_SECOND:
		SecondText();
		break;
	case CSkillManager::GRADE_THIRD:
		ThirdText();
		break;
	case CSkillManager::GRADE_FOURTH:
		FourthText();
		break;
	case CSkillManager::GRADE_FIFTH:
		FifthText();
		break;
	default:
		break;
	}
	
}

void CSkillFrame::BegennerText()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	wchar_t SkillPoint[10];
	_itow_s(m_pSkillManager->Get_SkillPoint(), SkillPoint, 10);

	wchar_t ReefAttackGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("ReefAttackInfo"), CSkillManager::GRADE_BEGENNER)->Get_SkillLevel(), ReefAttackGetLevel, 10);

	wchar_t WarriorReefGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("WarriorReefInfo"), CSkillManager::GRADE_BEGENNER)->Get_SkillLevel(), WarriorReefGetLevel, 10);

	RECT Grade;
	SetRect(&Grade, 865, 180, 0, 0);
	m_GradeFont->DrawText(NULL, m_cGrade, -1, &Grade, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));

	RECT SkillPointrt;
	SetRect(&SkillPointrt, m_iSkillPointDigit, 150, 0, 0);
	m_FrameFont->DrawText(NULL, SkillPoint, -1, &SkillPointrt, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f));

	RECT ReefAttackName;
	SetRect(&ReefAttackName, 792, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("ReefAttackInfo"), CSkillManager::GRADE_BEGENNER)->Get_SkillName(), -1, &ReefAttackName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT ReefAttackLevel;
	SetRect(&ReefAttackLevel, 793, 233, 0, 0);
	m_FrameFont->DrawText(NULL, ReefAttackGetLevel, -1, &ReefAttackLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT WarriorReefName;
	SetRect(&WarriorReefName, 935, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("WarriorReefInfo"), CSkillManager::GRADE_BEGENNER)->Get_SkillName(), -1, &WarriorReefName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT WarriorReefLevel;
	SetRect(&WarriorReefLevel, 936, 233, 0, 0);
	m_FrameFont->DrawText(NULL, WarriorReefGetLevel, -1, &WarriorReefLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));
}

void CSkillFrame::FirstText()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	wchar_t SkillPoint[10];
	_itow_s(m_pSkillManager->Get_SkillPoint(), SkillPoint, 10);

	wchar_t SunCrossGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("SunCrossInfo"), CSkillManager::GRADE_FIRST)->Get_SkillLevel(), SunCrossGetLevel, 10);

	wchar_t SolunaSlashGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("SolunaSlashInfo"), CSkillManager::GRADE_FIRST)->Get_SkillLevel(), SolunaSlashGetLevel, 10);

	RECT Grade;
	SetRect(&Grade, 865, 180, 0, 0);
	m_GradeFont->DrawText(NULL, m_cGrade, -1, &Grade, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));

	RECT SkillPointrt;
	SetRect(&SkillPointrt, m_iSkillPointDigit, 150, 0, 0);
	m_FrameFont->DrawText(NULL, SkillPoint, -1, &SkillPointrt, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f));

	RECT SuncrossName;
	SetRect(&SuncrossName, 792, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("SunCrossInfo"), CSkillManager::GRADE_FIRST)->Get_SkillName(), -1, &SuncrossName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT SunCrossLevel;
	SetRect(&SunCrossLevel, 793, 233, 0, 0);
	m_FrameFont->DrawText(NULL, SunCrossGetLevel, -1, &SunCrossLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT SolunaSlashName;
	SetRect(&SolunaSlashName, 935, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("SolunaSlashInfo"), CSkillManager::GRADE_FIRST)->Get_SkillName(), -1, &SolunaSlashName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT SolunaSlashLevel;
	SetRect(&SolunaSlashLevel, 936, 233, 0, 0);
	m_FrameFont->DrawText(NULL, SolunaSlashGetLevel, -1, &SolunaSlashLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));
}

void CSkillFrame::SecondText()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	wchar_t SkillPoint[10];
	_itow_s(m_pSkillManager->Get_SkillPoint(), SkillPoint, 10);

	wchar_t SpearPullingGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("SpearPullingInfo"), CSkillManager::GRADE_SECOND)->Get_SkillLevel(), SpearPullingGetLevel, 10);

	wchar_t CardinalBlastGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("CardinalBlastInfo"), CSkillManager::GRADE_SECOND)->Get_SkillLevel(), CardinalBlastGetLevel, 10);

	RECT Grade;
	SetRect(&Grade, 865, 180, 0, 0);
	m_GradeFont->DrawText(NULL, m_cGrade, -1, &Grade, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));

	RECT SkillPointrt;
	SetRect(&SkillPointrt, m_iSkillPointDigit, 150, 0, 0);
	m_FrameFont->DrawText(NULL, SkillPoint, -1, &SkillPointrt, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f));

	RECT SpearPullingName;
	SetRect(&SpearPullingName, 792, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("SpearPullingInfo"), CSkillManager::GRADE_SECOND)->Get_SkillName(), -1, &SpearPullingName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT SpearPullingLevel;
	SetRect(&SpearPullingLevel, 793, 233, 0, 0);
	m_FrameFont->DrawText(NULL, SpearPullingGetLevel, -1, &SpearPullingLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT CardinalBlastName;
	SetRect(&CardinalBlastName, 935, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("CardinalBlastInfo"), CSkillManager::GRADE_SECOND)->Get_SkillName(), -1, &CardinalBlastName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT CardinalBlastLevel;
	SetRect(&CardinalBlastLevel, 936, 233, 0, 0);
	m_FrameFont->DrawText(NULL, CardinalBlastGetLevel, -1, &CardinalBlastLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));
}

void CSkillFrame::ThirdText()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	wchar_t SkillPoint[10];
	_itow_s(m_pSkillManager->Get_SkillPoint(), SkillPoint, 10);

	RECT Grade;
	SetRect(&Grade, 865, 180, 0, 0);
	m_GradeFont->DrawText(NULL, m_cGrade, -1, &Grade, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));

	RECT SkillPointrt;
	SetRect(&SkillPointrt, m_iSkillPointDigit, 150, 0, 0);
	m_FrameFont->DrawText(NULL, SkillPoint, -1, &SkillPointrt, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f));

	wchar_t ChasingShotGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("ChasingShotInfo"), CSkillManager::GRADE_THIRD)->Get_SkillLevel(), ChasingShotGetLevel, 10);

	RECT ChasingShotName;
	SetRect(&ChasingShotName, 792, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("ChasingShotInfo"), CSkillManager::GRADE_THIRD)->Get_SkillName(), -1, &ChasingShotName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT ChasingShotLevel;
	SetRect(&ChasingShotLevel, 793, 233, 0, 0);
	m_FrameFont->DrawText(NULL, ChasingShotGetLevel, -1, &ChasingShotLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));
}

void CSkillFrame::FourthText()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	wchar_t SkillPoint[10];
	_itow_s(m_pSkillManager->Get_SkillPoint(), SkillPoint, 10);

	RECT Grade;
	SetRect(&Grade, 865, 180, 0, 0);
	m_GradeFont->DrawText(NULL, m_cGrade, -1, &Grade, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));

	RECT SkillPointrt;
	SetRect(&SkillPointrt, m_iSkillPointDigit, 150, 0, 0);
	m_FrameFont->DrawText(NULL, SkillPoint, -1, &SkillPointrt, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f));

	wchar_t BeastGetLevel[10];
	_itow_s(pSkillInstance->Get_SkillInfo(TEXT("BeastInfo"), CSkillManager::GRADE_FOURTH)->Get_SkillLevel(), BeastGetLevel, 10);

	RECT BeastName;
	SetRect(&BeastName, 792, 215, 0, 0);
	m_FrameFont->DrawText(NULL, pSkillInstance->Get_SkillInfo(TEXT("BeastInfo"), CSkillManager::GRADE_FOURTH)->Get_SkillName(), -1, &BeastName, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));

	RECT BeastLevel;
	SetRect(&BeastLevel, 793, 233, 0, 0);
	m_FrameFont->DrawText(NULL, BeastGetLevel, -1, &BeastLevel, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.0f, 1.0f));
}

void CSkillFrame::FifthText()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	wchar_t SkillPoint[10];
	_itow_s(m_pSkillManager->Get_SkillPoint(), SkillPoint, 10);

	RECT Grade;
	SetRect(&Grade, 865, 180, 0, 0);
	m_GradeFont->DrawText(NULL, m_cGrade, -1, &Grade, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.0f, 1.0f));

	RECT SkillPointrt;
	SetRect(&SkillPointrt, m_iSkillPointDigit, 150, 0, 0);
	m_FrameFont->DrawText(NULL, SkillPoint, -1, &SkillPointrt, DT_NOCLIP, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f));
}

void CSkillFrame::Set_Data()
{
	if (m_pSkillManager->Get_SkillPoint() < 10)
	{
		m_iSkillPointDigit = 1040;
	}
	else if (m_pSkillManager->Get_SkillPoint() < 100)
	{
		m_iSkillPointDigit = 1030;
	}
	else
	{
		m_iSkillPointDigit = 1020;
	}

	switch (m_pSkillManager->Get_SkillGrade())
	{
	case CSkillManager::GRADE_BEGENNER:
		m_cGrade = L"0차 전직";
		break;
	case CSkillManager::GRADE_FIRST:
		m_cGrade = L"1차 전직";
		break;
	case CSkillManager::GRADE_SECOND:
		m_cGrade = L"2차 전직";
		break;
	case CSkillManager::GRADE_THIRD:
		m_cGrade = L"3차 전직";
		break;
	case CSkillManager::GRADE_FOURTH:
		m_cGrade = L"4차 전직";
		break;
	case CSkillManager::GRADE_FIFTH:
		m_cGrade = L"5차 전직";
		break;

	default:
		break;
	}
}

void CSkillFrame::Free()
{
	__super::Free();

}



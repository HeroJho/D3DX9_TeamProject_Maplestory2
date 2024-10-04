#include "stdafx.h"
#include "..\Public\QuickSlotSkill.h"
#include "GameInstance.h"
#include "QuickSlotManager.h"
#include "MouseManager.h"
#include "SkillInfo.h"
#include "UIManager.h"



CQuickSlotSkill::CQuickSlotSkill(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CQuickSlotSkill::CQuickSlotSkill(const CQuickSlotSkill& rhs)
	: CUI(rhs)
{
}


HRESULT CQuickSlotSkill::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CQuickSlotSkill::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));
	m_iIndexNum = m_UIInfo.iNum;
	
	
	__super::Initialize(pArg);
	m_iTexturenum = 99;
	m_eGrade = CSkillManager::GRADE_END;
	m_pSkillInfoTag = nullptr;
	m_pSkillNotice = nullptr;
	m_bRender = false;
	CQuickSlotManager* pQuickSlotInstance = CQuickSlotManager::Get_Instance();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseSkillIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	pQuickSlotInstance->Add_QuickSlotSkill(this);

	D3DXCreateFont(m_pGraphic_Device, 13, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_NoticeFont);


	return S_OK;
}

void CQuickSlotSkill::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CQuickSlotManager* pQuickSlotInstance = CQuickSlotManager::Get_Instance();
	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();

	Check_Collision(DIMK_LBUTTON);

	Change_Texture();

	if (pGameInstance->Mouse_Up(DIMK_LBUTTON) && m_eCollision == TYPE_NO && pQuickSlotInstance->Check_Delete() && pMouseInstance->Get_PickType() == CMouseManager::TYPE_QUICK)
	{
		pQuickSlotInstance->Clear_Data(pMouseInstance->Get_Indexnum());
	}
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);



	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);
	Safe_Release(pGameInstance);
}

void CQuickSlotSkill::LateTick(_float fTimeDelta)
{
	
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CQuickSlotSkill::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;

	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();


	return S_OK;
}

void CQuickSlotSkill::Change_Texture()
{
	CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
	CQuickSlotManager* pQuickSlotInstance = CQuickSlotManager::Get_Instance();

	switch (m_eCollision)
	{
	case Client::CUI::TYPE_NO:
		break;
	case Client::CUI::TYPE_ON:

		break;
	case Client::CUI::TYPE_DOWN:
		CGameInstance::Get_Instance()->PlaySoundW(L"DragStart.mp3", 25, 1.f);
		pMouseInstance->Set_SkillIconIndex(CMouseManager::TYPE_QUICK, m_pSkillInfoTag, m_eGrade, m_iTexturenum, m_pSkillNotice, m_iIndexNum);
		break;
	case Client::CUI::TYPE_UP:	
		if (!pQuickSlotInstance->Check_SkillTexture(pMouseInstance->Get_SkillIconTextnum()))
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragEnd.mp3", 25, 1.f);
			m_pSkillInfoTag = pMouseInstance->Get_SkillInfoTag();
			m_eGrade = pMouseInstance->Get_Grade();
			m_iTexturenum = pMouseInstance->Get_SkillIconTextnum();
			m_pSkillNotice = pMouseInstance->Get_SkillNotice();
			
		}
		else
		{
			if (pMouseInstance->Get_PickType() == CMouseManager::TYPE_QUICK)
			{
				CGameInstance::Get_Instance()->PlaySoundW(L"DragEnd.mp3", 25, 1.f);
				pQuickSlotInstance->Change_SkillSlot(pMouseInstance->Get_Indexnum(), this);
				m_pSkillInfoTag = pMouseInstance->Get_SkillInfoTag();
				m_eGrade = pMouseInstance->Get_Grade();
				m_iTexturenum = pMouseInstance->Get_SkillIconTextnum();
				m_pSkillNotice = pMouseInstance->Get_SkillNotice();
			}			
		}
		m_bRender = true;
		break;
	case Client::CUI::TYPE_PRESSING:
		break;
	case Client::CUI::TYPE_END:
		break;
	default:
		break;
	}

}

void CQuickSlotSkill::Clear_Data()
{
	m_iTexturenum = 99;
	m_pSkillInfoTag = nullptr;
	m_pSkillNotice = nullptr;
	m_eGrade = CSkillManager::GRADE_END;
}


	


CQuickSlotSkill* CQuickSlotSkill::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQuickSlotSkill*		pInstance = new CQuickSlotSkill(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CQuickSlotSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuickSlotSkill::Clone(void * pArg)
{
	CQuickSlotSkill*		pInstance = new CQuickSlotSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CQuickSlotSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuickSlotSkill::Free()
{
	__super::Free();

}

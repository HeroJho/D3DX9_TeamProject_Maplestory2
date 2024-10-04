#include "stdafx.h"
#include "..\Public\SunCrossIcon.h"
#include "GameInstance.h"
#include "SkillManager.h"
#include "MouseManager.h"


CSunCrossIcon::CSunCrossIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CSunCrossIcon::CSunCrossIcon(const CSunCrossIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CSunCrossIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CSunCrossIcon::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));

	__super::Initialize(pArg);
	m_iTexturenum = 0;
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	m_pSkillInfo = (CSunCrossInfo*)pSkillInstance->Get_SkillInfo(L"SunCrossInfo", CSkillManager::GRADE_FIRST);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SunCrossIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 13, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_NoticeFont);

	pSkillInstance->Add_SkillFrameImage(this);

	m_bRender = false;

	return S_OK;
}

void CSunCrossIcon::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	if (pInstance->Key_Down(DIK_K))
	{
		m_bRender = !m_bRender;
	}

	if (pInstance->Mouse_Up(DIMK_LBUTTON))
	{

	}
	Check_Collision(DIMK_LBUTTON);

	Safe_Release(pInstance);
}

void CSunCrossIcon::LateTick(_float fTimeDelta)
{
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	Change_Texture();
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	if (m_bRender&& pSkillInstance->Get_SkillGrade() == CSkillManager::GRADE_FIRST)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	
}

HRESULT CSunCrossIcon::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexturenum)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();


	return S_OK;
}

void CSunCrossIcon::Change_Texture()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	if (pSkillInstance->Get_SkillInfo(L"SunCrossInfo", CSkillManager::GRADE_FIRST)->Get_SkillLevel() < 1)
		m_iTexturenum = 0;
	else
		m_iTexturenum = 1;

	if (m_bRender)
	{
		CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
		if (m_eCollision == TYPE_DOWN && m_pSkillInfo->Get_SkillLevel() != 0 && pSkillInstance->Get_SkillGrade() == CSkillManager::GRADE_FIRST)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragStart.mp3", 25, 1.f);
			pMouseInstance->Set_SkillIconIndex(CMouseManager::TYPE_SKILL, L"SunCrossInfo", CSkillManager::GRADE_FIRST, m_pSkillInfo->Get_TextNum(), m_pSkillInfo->Get_SkillNotice());
		}
	}
	
}

CSunCrossIcon* CSunCrossIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSunCrossIcon*		pInstance = new CSunCrossIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSunCrossIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSunCrossIcon::Clone(void * pArg)
{
	CSunCrossIcon*		pInstance = new CSunCrossIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSunCrossIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSunCrossIcon::Free()
{
	__super::Free();
}

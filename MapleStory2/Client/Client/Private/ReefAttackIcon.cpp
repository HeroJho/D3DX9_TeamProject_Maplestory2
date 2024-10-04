#include "stdafx.h"
#include "..\Public\ReefAttackIcon.h"
#include "GameInstance.h"
#include "SkillManager.h"
#include "MouseManager.h"


CReefAttackIcon::CReefAttackIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CReefAttackIcon::CReefAttackIcon(const CReefAttackIcon& rhs)
	: CUI(rhs)
{
}


HRESULT CReefAttackIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CReefAttackIcon::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));

	__super::Initialize(pArg);
	m_iTexturenum = 0;
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	m_pSkillInfo = (CReefAttackInfo*)pSkillInstance->Get_SkillInfo(L"ReefAttackInfo", CSkillManager::GRADE_BEGENNER);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ReefAttackIcon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 13, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"µ¸¿òÃ¼", &m_NoticeFont);

	pSkillInstance->Add_SkillFrameImage(this);

	m_bRender = false;

	return S_OK;
}

void CReefAttackIcon::Tick(_float fTimeDelta)
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

void CReefAttackIcon::LateTick(_float fTimeDelta)
{
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	Change_Texture();
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	if (m_bRender&& pSkillInstance->Get_SkillGrade() == CSkillManager::GRADE_BEGENNER)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CReefAttackIcon::Render()
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

void CReefAttackIcon::Change_Texture()
{
	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	if (pSkillInstance->Get_SkillInfo(L"ReefAttackInfo", CSkillManager::GRADE_BEGENNER)->Get_SkillLevel() < 1)
		m_iTexturenum = 0;
	else
		m_iTexturenum = 1;

	if (m_bRender)
	{
		CMouseManager* pMouseInstance = CMouseManager::Get_Instance();
		if (m_eCollision == TYPE_DOWN && m_pSkillInfo->Get_SkillLevel() != 0)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"DragStart.mp3", 25, 1.f);
			pMouseInstance->Set_SkillIconIndex(CMouseManager::TYPE_SKILL, L"ReefAttackInfo", CSkillManager::GRADE_BEGENNER, m_pSkillInfo->Get_TextNum(), m_pSkillInfo->Get_SkillNotice());
		}
	}
}

CReefAttackIcon* CReefAttackIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CReefAttackIcon*		pInstance = new CReefAttackIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CReefAttackIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CReefAttackIcon::Clone(void * pArg)
{
	CReefAttackIcon*		pInstance = new CReefAttackIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CReefAttackIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CReefAttackIcon::Free()
{
	__super::Free();
}

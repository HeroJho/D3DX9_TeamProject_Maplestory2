#include "stdafx.h"
#include "..\Public\SkillGradeBtn1.h"
#include "GameInstance.h"
#include "SkillManager.h"


CSkillGradeBtn1::CSkillGradeBtn1(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CSkillGradeBtn1::CSkillGradeBtn1(const CSkillGradeBtn1& rhs)
	: CUI(rhs)
{
}


HRESULT CSkillGradeBtn1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CSkillGradeBtn1::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));
	m_iTexturenum = 0;
	__super::Initialize(pArg);

	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkillGradeBtn1"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	pSkillInstance->Add_SkillFrameImage(this);

	m_bRender = false;

	return S_OK;
}

void CSkillGradeBtn1::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	if (pInstance->Key_Down(DIK_K))
	{
		m_bRender = !m_bRender;
	}

	Check_Collision(DIMK_LBUTTON);
	Change_Texture();

	Safe_Release(pInstance);
}

void CSkillGradeBtn1::LateTick(_float fTimeDelta)
{
	

	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));	

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);


}

HRESULT CSkillGradeBtn1::Render()
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

void CSkillGradeBtn1::Change_Texture()
{

	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();
	if (pSkillInstance->Get_PlayerGrade() < 1)
	{
		m_iTexturenum = 0;
		return;
	}

	switch (m_eCollision)
	{
	case Client::CUI::TYPE_NO:
		break;
	case Client::CUI::TYPE_ON:		
		break;
	case Client::CUI::TYPE_DOWN:
		CGameInstance::Get_Instance()->PlaySoundW(L"Tab.mp3", 25, 1.f);
		pSkillInstance->Set_SkillGrade(CSkillManager::GRADE_FIRST);
		break;
	case Client::CUI::TYPE_UP:
		break;
	case Client::CUI::TYPE_PRESSING:	
		break;
	case Client::CUI::TYPE_END:
		break;
	default:
		break;
	}
	
	if (pSkillInstance->Get_SkillGrade() == CSkillManager::GRADE_FIRST)
		m_iTexturenum = 2;
	else
		m_iTexturenum = 1;
	

	

}

CSkillGradeBtn1* CSkillGradeBtn1::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkillGradeBtn1*		pInstance = new CSkillGradeBtn1(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSkillGradeBtn1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillGradeBtn1::Clone(void * pArg)
{
	CSkillGradeBtn1*		pInstance = new CSkillGradeBtn1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSkillGradeBtn1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkillGradeBtn1::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\SkillGradeBtn5.h"
#include "GameInstance.h"
#include "SkillManager.h"


CSkillGradeBtn5::CSkillGradeBtn5(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CSkillGradeBtn5::CSkillGradeBtn5(const CSkillGradeBtn5& rhs)
	: CUI(rhs)
{
}


HRESULT CSkillGradeBtn5::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CSkillGradeBtn5::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));
	m_iTexturenum = 0;
	__super::Initialize(pArg);

	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkillGradeBtn5"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	pSkillInstance->Add_SkillFrameImage(this);

	m_bRender = false;

	return S_OK;
}

void CSkillGradeBtn5::Tick(_float fTimeDelta)
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

void CSkillGradeBtn5::LateTick(_float fTimeDelta)
{
	

	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));	

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);


}

HRESULT CSkillGradeBtn5::Render()
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

void CSkillGradeBtn5::Change_Texture()
{

	CSkillManager* pSkillInstance = CSkillManager::Get_Instance();

	if (pSkillInstance->Get_PlayerGrade() < 5)
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
		pSkillInstance->Set_SkillGrade(CSkillManager::GRADE_FIFTH);
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
	
	if (pSkillInstance->Get_SkillGrade() == CSkillManager::GRADE_FIFTH)
		m_iTexturenum = 2;
	else
		m_iTexturenum = 1;
	

	

}

CSkillGradeBtn5* CSkillGradeBtn5::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkillGradeBtn5*		pInstance = new CSkillGradeBtn5(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSkillGradeBtn5"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillGradeBtn5::Clone(void * pArg)
{
	CSkillGradeBtn5*		pInstance = new CSkillGradeBtn5(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSkillGradeBtn5"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkillGradeBtn5::Free()
{
	__super::Free();

}

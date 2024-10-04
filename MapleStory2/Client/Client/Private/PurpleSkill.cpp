#include "stdafx.h"
#include "..\Public\PurpleSkill.h"
#include "GameInstance.h"
#include "QuestManager.h"'
#include "Player.h"

CPurpleSkill::CPurpleSkill(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_UIInfo, sizeof(UIINFO));
}

CPurpleSkill::CPurpleSkill(const CPurpleSkill & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPurpleSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPurpleSkill::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f);

	m_UIInfo.fSizeX = 2048.f;
	m_UIInfo.fSizeY = 1024.f;
	m_UIInfo.fX = g_iWinSizeX / 2;
	m_UIInfo.fY = g_iWinSizeY / 2;

	m_pTransformCom->Set_Scaled(_float3(m_UIInfo.fSizeX, m_UIInfo.fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));




	m_fTime = 0;

	return S_OK;
}

void CPurpleSkill::Tick(_float fTimeDelta)
{
	
	RECT		rcUI;
	SetRect(&rcUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);


	m_fTime += fTimeDelta;

	

}

void CPurpleSkill::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPurpleSkill::Render()
{
	_float4x4		Matrix;
	D3DXMatrixIdentity(&Matrix);
	CQuestManager* Questinstance = CQuestManager::Get_Instance();


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_fTime < 0.2f)
		m_pTextureCom->Bind_Texture(0);
	else if (0.2f <= m_fTime && m_fTime < 0.4f)
		m_pTextureCom->Bind_Texture(1);
	else if (0.4f <= m_fTime && m_fTime< 0.6f)
		m_pTextureCom->Bind_Texture(2);
	else if (0.6f <= m_fTime && m_fTime< 0.8f)
		m_pTextureCom->Bind_Texture(3);
	else if (0.8f <= m_fTime && m_fTime< 1.f)
		m_pTextureCom->Bind_Texture(4);
	else if (1.f <= m_fTime && m_fTime< 1.2f)
		m_pTextureCom->Bind_Texture(5);
	else if (1.2f <= m_fTime && m_fTime< 1.4f)
		m_pTextureCom->Bind_Texture(6);
	else if (1.4f <= m_fTime && m_fTime< 1.6f)
		m_pTextureCom->Bind_Texture(7);
	else if (1.6f <= m_fTime && m_fTime< 1.8f)
		m_pTextureCom->Bind_Texture(8);
	else if (1.8f <= m_fTime && m_fTime < 2.f)
		m_pTextureCom->Bind_Texture(9);
	else if (2.f <= m_fTime && m_fTime< 2.2f)
		m_pTextureCom->Bind_Texture(10);
	else if (2.2f <= m_fTime && m_fTime< 2.4f)
		m_pTextureCom->Bind_Texture(11);
	else if (2.4f <= m_fTime && m_fTime< 2.6f)
		m_pTextureCom->Bind_Texture(12);
	else if (2.6f <= m_fTime && m_fTime< 2.8f)
		m_pTextureCom->Bind_Texture(13);
	else if (2.8f <= m_fTime && m_fTime< 3.f)
		m_pTextureCom->Bind_Texture(14);
	else if (3.f <= m_fTime && m_fTime< 3.2f)
		m_pTextureCom->Bind_Texture(15);
	else if (3.2f <= m_fTime && m_fTime< 3.4f)
		m_pTextureCom->Bind_Texture(16);
	else if (3.4f <= m_fTime && m_fTime< 3.6f)
		m_pTextureCom->Bind_Texture(17);
	else if(3.6f <= m_fTime &&  m_fTime < 3.8f)
		m_pTextureCom->Bind_Texture(18);


	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if(m_fTime < 3.8f)
		m_pVIBufferCom->Render();
	else
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		((CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_STATIC, L"Layer_Player", 0))->Player_Cut();

		Safe_Release(pGameInstance);
		Set_Dead();
	}


	return S_OK;
}

HRESULT CPurpleSkill::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PurpleSkill"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

void CPurpleSkill::MouseCollision()
{
}

CPurpleSkill * CPurpleSkill::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPurpleSkill*		pInstance = new CPurpleSkill(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPurpleSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPurpleSkill::Clone(void* pArg)
{
	CPurpleSkill*		pInstance = new CPurpleSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CPurpleSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPurpleSkill::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


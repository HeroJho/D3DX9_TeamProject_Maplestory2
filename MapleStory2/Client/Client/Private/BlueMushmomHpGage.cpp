#include "stdafx.h"
#include "..\Public\BlueMushmomHpGage.h"
#include "GameInstance.h"
#include "QuestManager.h"
#include "UIManager.h"

CBlueMushmomHpGage::CBlueMushmomHpGage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_UIInfo, sizeof(UIINFO));
}

CBlueMushmomHpGage::CBlueMushmomHpGage(const CBlueMushmomHpGage & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBlueMushmomHpGage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlueMushmomHpGage::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0, 1);

	m_UIInfo.fSizeX = 608.f;
	m_UIInfo.fSizeY = 21.f;
	m_UIInfo.fX = 613.0f;
	m_UIInfo.fY = 20.0f;



	m_pTransformCom->Set_Scaled(_float3(m_UIInfo.fSizeX, m_UIInfo.fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	m_fOriginHp = _float(CUIManager::Get_Instance()->Set_BlueMushmomHp());
	m_fNowHp = 0;

	m_fOriginSize = m_UIInfo.fSizeX;
	

	
	return S_OK;
}

void CBlueMushmomHpGage::Tick(_float fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	RECT		rcUI;
	SetRect(&rcUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);

}

void CBlueMushmomHpGage::LateTick(_float fTimeDelta)
{
	m_fNowHp = _float(CUIManager::Get_Instance()->Set_BlueMushmomHp());

	m_UIInfo.fSizeX = m_fOriginSize * (m_fNowHp / m_fOriginHp);

	_float fTemp = (m_fOriginSize - m_fOriginSize * (m_fNowHp / m_fOriginHp)) / 2;

	m_pTransformCom->Set_Scaled(_float3(m_UIInfo.fSizeX, m_UIInfo.fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f - fTemp, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CBlueMushmomHpGage::Render()
{

	_float4x4		Matrix;
	D3DXMatrixIdentity(&Matrix);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	// 비어있는 퀘스트 ui를 출력
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;



	//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Matrix);
	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);


	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (vPlayerPos.x > 30.f && (_float(CUIManager::Get_Instance()->Set_BlueMushmomHp()) > 0))
		m_pVIBufferCom->Render();

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBlueMushmomHpGage::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	
	return S_OK;
}

void CBlueMushmomHpGage::MouseCollision()
{
}

CBlueMushmomHpGage * CBlueMushmomHpGage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlueMushmomHpGage*		pInstance = new CBlueMushmomHpGage(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlueMushmomHpGage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlueMushmomHpGage::Clone(void* pArg)
{
	CBlueMushmomHpGage*		pInstance = new CBlueMushmomHpGage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CBlueMushmomHpGage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlueMushmomHpGage::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


#include "stdafx.h"
#include "..\Public\TombUI.h"
#include "GameInstance.h"
#include "QuestManager.h"
#include "UI.h"
#include "Player.h"
#include "Model_Die.h"

CTombUI::CTombUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_UIInfo, sizeof(UIINFO));
}

CTombUI::CTombUI(const CTombUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTombUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTombUI::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 2.f);

	m_UIInfo.fSizeX = 300.f;
	m_UIInfo.fSizeY = 131.f;
	m_UIInfo.fX = g_iWinSizeX / 2;
	m_UIInfo.fY = g_iWinSizeY / 2 - 100;

	m_pTransformCom->Set_Scaled(_float3(m_UIInfo.fSizeX, m_UIInfo.fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));




	m_fTime = 0;

	return S_OK;
}

void CTombUI::Tick(_float fTimeDelta)
{
	
	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);


	MouseCollision(DIMK_LBUTTON);
}

void CTombUI::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}


HRESULT CTombUI::Render()
{
	_float4x4		Matrix;
	D3DXMatrixIdentity(&Matrix);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);



	switch (m_eCollision)
	{
	case Client::CUI::TYPE_NO:
		m_pTextureCom->Bind_Texture(0);
		break;
	case Client::CUI::TYPE_ON:
		m_pTextureCom->Bind_Texture(1);
		break;
	case Client::CUI::TYPE_DOWN:
		m_pTextureCom->Bind_Texture(2);
		break;
	case Client::CUI::TYPE_UP:
		m_pTextureCom->Bind_Texture(2);
		CGameInstance::Get_Instance()->PlaySoundW(L"BtMouseClick.mp3", 25, 1.f);
		((CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_STATIC, L"Layer_Player", 0))->Player_Revive();
		((CPlayer*)pGameInstance->Get_ObjectPtr(LEVEL_STATIC, L"Layer_Player", 0))->Player_NoneDieing();
		CUIManager::Get_Instance()->Full_PlayerHp();
		Set_Dead();
		break;
	case Client::CUI::TYPE_PRESSING:
		m_pTextureCom->Bind_Texture(2);
		break;
	case Client::CUI::TYPE_END:
		break;
	default:
		break;
	}

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	//if(m_fTime < 3.8f)
		m_pVIBufferCom->Render();

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CTombUI::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TombUI"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

void CTombUI::MouseCollision(DIMK m_eMouseKey)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (PtInRect(&m_RectUI, ptMouse))
	{
		if (pGameInstance->Mouse_Down(m_eMouseKey))
		{
			m_eCollision = TYPE_DOWN;

		}
		else if (pGameInstance->Mouse_Up(m_eMouseKey))
		{
			m_eCollision = TYPE_UP;

		}
		else if (pGameInstance->Mouse_Pressing(m_eMouseKey))
		{
			m_eCollision = TYPE_PRESSING;

		}
		else
		{
			m_eCollision = TYPE_ON;

		}
	}
	else
	{
		m_eCollision = TYPE_NO;

	}
	Safe_Release(pGameInstance);
}

CTombUI * CTombUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTombUI*		pInstance = new CTombUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTombUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTombUI::Clone(void* pArg)
{
	CTombUI*		pInstance = new CTombUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CTombUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTombUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


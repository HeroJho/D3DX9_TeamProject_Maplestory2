#include "stdafx.h"
#include "..\Public\UI.h"
#include "GameInstance.h"
#include "UIManager.h"


CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_UIInfo, sizeof(UIINFO));
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}



HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void * pArg)
{

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0, 1);

	SetRect(&m_RectUI, m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f, m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f, m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f);
	m_bRender = true;
	m_iTexturenum = 0;
	m_eCollision = TYPE_NO;
	D3DXMatrixIdentity(&m_ViewMatrix);
	m_pTransformCom->Set_Scaled(_float3(m_UIInfo.fSizeX, m_UIInfo.fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_OriginInfo.fX = m_UIInfo.fX;
	m_OriginInfo.fY = m_UIInfo.fY;
	_float3 vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vOriginPos = vOriginPos;
	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
}

void CUI::LateTick(_float fTimeDelta)
{
}

HRESULT CUI::Render()
{

	return S_OK;
}

void CUI::Start_CutScene(_float fTimeDelta)
{
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y > m_vOriginPos.y - 200.f)
	{
		m_UIInfo.fY += 4.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	}
	else
	{
		CUIManager::Get_Instance()->Set_StartMove(false);
	}
}

void CUI::End_CutScene(_float fTimeDelta)
{
	
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < m_vOriginPos.y)
	{
		m_UIInfo.fY -= 4.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	}
	else
	{
		CUIManager::Get_Instance()->Set_EndMove(false);
	}
}

HRESULT CUI::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	return S_OK;
}

HRESULT CUI::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CUI::Check_Collision(DIMK m_eMouseKey)
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

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI*		pInstance = new CUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCreature"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI::Clone(void * pArg)
{
	CUI*		pInstance = new CUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

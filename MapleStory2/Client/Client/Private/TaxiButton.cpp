#include "stdafx.h"
#include "..\Public\TaxiButton.h"

#include "GameInstance.h"
#include "ToolManager.h"
#include "UIManager.h"
#include "CutSceneManager.h"


CTaxiButton::CTaxiButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CTaxiButton::CTaxiButton(const CTaxiButton& rhs)
	: CUI(rhs)
{
}





HRESULT CTaxiButton::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CTaxiButton::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_Desc = *(TAXIBUTTONDESC*)pArg;

	m_UIInfo.fSizeX = 150.f;
	m_UIInfo.fSizeY = 50.f;
	m_UIInfo.fX = m_Desc.vPos.x;
	m_UIInfo.fY = m_Desc.vPos.y;
	m_iTexturenum = 0;

	__super::Initialize(pArg);



	m_bRender = true;

	m_OnColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	D3DXCreateFont(m_pGraphic_Device, 29, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"), &m_pFont);


	return S_OK;
}





void CTaxiButton::Tick(_float fTimeDelta)
{

	Check_Collision(DIMK_LBUTTON);

}

void CTaxiButton::LateTick(_float fTimeDelta)
{
	if (m_eCollision == TYPE_DOWN)
	{
		CGameInstance::Get_Instance()->PlaySoundW(L"TaxiMove.mp3", 21, 1.f);
		CUIManager::Get_Instance()->Start_Loading();
		m_bTrigger = true;
		CUIManager::Get_Instance()->Off_TaxiChatBox();
		CCutSceneManager::Get_Instance()->Get_MainCam()->SetCamMode(CCamera_Free::CAM_PLAYER);
	}
	else if (m_eCollision == TYPE_ON)
		m_OnColor = D3DXCOLOR(255.f, 127.f, 0.f, 1.f);
	else
		m_OnColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);;

	// m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);


	if (m_bTrigger)
	{
		m_fTimeAcc += fTimeDelta;

		if (5.f < m_fTimeAcc)
		{
			// 마을로 이동
			switch (m_Desc.eLevel)
			{
			case LEVEL_ELENYAENTER:
				CToolManager::Get_Instance()->SetDestLevel(LEVEL_ELENYAENTER, _float3(26.f, 1.f, -10.f));
				break;
			case LEVEL_ELENYA:
				CToolManager::Get_Instance()->SetDestLevel(LEVEL_ELENYA, _float3(-1.f, 1.f, 0.f));
				break;
			}

			m_bTrigger = false;
		}
	}

}

HRESULT CTaxiButton::Render()
{
	if (!m_bRender)
		return S_OK;



	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	//if (FAILED(m_pTextureCom->Bind_Texture(m_iCount)))
	//	return E_FAIL;

	//Set_RenderState();

	//m_pVIBufferCom->Render();

	//Reset_RenderState();


	RECT rc;

	TCHAR cChat[128];

	switch (m_Desc.eLevel)
	{
		case LEVEL_ELENYAENTER:
		{

			wsprintf(cChat, TEXT("1. 엘리니아로 가는 길"));
			SetRect(&rc, m_Desc.vPos.x - 30.f, m_Desc.vPos.y, 0, 0);
			m_pFont->DrawText(NULL, cChat,
				-1, &rc, DT_NOCLIP, m_OnColor);

		}
			break;
		case LEVEL_ELENYA:
		{

			wsprintf(cChat, TEXT("2. 엘리니아"));
			SetRect(&rc, m_Desc.vPos.x - 30.f, m_Desc.vPos.y, 0, 0);
			m_pFont->DrawText(NULL, cChat,
				-1, &rc, DT_NOCLIP, m_OnColor);

		}
			break;
	}






	return S_OK;
}







HRESULT CTaxiButton::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;


	return S_OK;
}







CTaxiButton* CTaxiButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTaxiButton*		pInstance = new CTaxiButton(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTaxiButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTaxiButton::Clone(void * pArg)
{
	CTaxiButton*		pInstance = new CTaxiButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTaxiButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTaxiButton::Free()
{
	__super::Free();

}

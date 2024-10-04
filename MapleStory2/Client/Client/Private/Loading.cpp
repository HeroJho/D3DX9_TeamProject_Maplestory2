#include "stdafx.h"
#include "..\Public\Loading.h"
#include "GameInstance.h"
#include "SkillManager.h"
#include "SunCrossInfo.h"
#include "Loading_Mop.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CLoading::CLoading(const CLoading& rhs)
	: CUI(rhs)
{
}


HRESULT CLoading::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CLoading::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = g_iWinSizeX;
	m_UIInfo.fSizeY = g_iWinSizeY;
	m_UIInfo.fX = g_iWinSizeX*0.5f;
	m_UIInfo.fY = g_iWinSizeY*0.5f;
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_iTexturenum = 0;

	__super::Initialize(pArg);


	m_fMaxCount = 15;
	m_fTime = 0.02f;
	m_bRender = true;




	D3DXCreateFont(m_pGraphic_Device, 21, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"), &m_pFont);
	D3DXCreateFont(m_pGraphic_Device, 25, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Tip"), &m_pTipFont);

	
	m_fRanNum = CGameInstance::Get_Instance()->Get_Random(0, 1);


	return S_OK;
}

void CLoading::Tick(_float fTimeDelta)
{
	//if (CGameInstance::Get_Instance()->Key_Down(DIK_M))
	//{
	//	if (m_bOn)
	//		EndLoading();
	//	else
	//		StartLoading();
	//}




	m_pMop->Tick(fTimeDelta);

	if (m_bOn)
		TickStart(fTimeDelta);
	else
		TickEnd(fTimeDelta);


	m_fTimeRenAcc += fTimeDelta;
	if (5.f < m_fTimeRenAcc)
	{
		m_fRanNum = CGameInstance::Get_Instance()->Get_Random(0, 1);

		m_fTimeRenAcc = 0.f;
	}

}

void CLoading::LateTick(_float fTimeDelta)
{
	if(m_bMop)
		m_pMop->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLoading::Render()
{
	if (!m_bRender)
		return S_OK;

	

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_Texture(m_fCount)))
		return E_FAIL;

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();


	if (!m_bMop)
		return S_OK;


	m_pMop->Render();
	





	switch (m_fRanNum)
	{
	case 0:
		{
			RECT rc;
			TCHAR cTip[128];
			wsprintf(cTip, TEXT("TIP."));
			SetRect(&rc, g_iWinSizeX*0.5f - 200.f, g_iWinSizeY*0.5f + 10.f, 0, 0);
			m_pFont->DrawText(NULL, cTip,
				-1, &rc, DT_NOCLIP, D3DXCOLOR(255.f, 127.f, 0.f, 1.f));

			TCHAR cChat[128];
			// wsprintf(cChat, TEXT("주황 버섯은 꾸준히 움직이지 않으면 버섯 집이 됩니다."));
			wsprintf(cChat, TEXT("리소스가 풍부한 게임을 선택하는게 중요합니다."));
			SetRect(&rc, g_iWinSizeX*0.5f - 160.f, g_iWinSizeY*0.5f + 10.f, 0, 0);
			m_pFont->DrawText(NULL, cChat,
				-1, &rc, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.f));
			m_pMop->SetImg(CLoading_Mop::IMG_MUSHROOM);
		}
			break;
	case 1:
		{
			RECT rc;
			TCHAR cTip[128];
			wsprintf(cTip, TEXT("TIP."));
			SetRect(&rc, g_iWinSizeX*0.5f - 200.f, g_iWinSizeY*0.5f + 10.f, 0, 0);
			m_pFont->DrawText(NULL, cTip,
				-1, &rc, DT_NOCLIP, D3DXCOLOR(255.f, 127.f, 0.f, 1.f));

			TCHAR cChat[128];
			// wsprintf(cChat, TEXT("슬라임은 위험에 처하면 땅속에 스며들어 숨습니다."));
			wsprintf(cChat, TEXT("수업을 들은 후에는 복습을 꼭 해야합니다."));
			SetRect(&rc, g_iWinSizeX*0.5f - 160.f, g_iWinSizeY*0.5f + 10.f, 0, 0);
			m_pFont->DrawText(NULL, cChat,
				-1, &rc, DT_NOCLIP, D3DXCOLOR(255.f, 255.f, 255.f, 1.f));
			m_pMop->SetImg(CLoading_Mop::IMG_SLIME);
		}
			break;
	}







	return S_OK;
}

void CLoading::StartLoading()
{
	if (m_bOn)
		return;

	m_bOn = true;
	m_bRender = true;
	m_bMop = false;
	m_fCount = m_fMaxCount;
	m_fTimeAcc = 0.f;
}

void CLoading::EndLoading()
{
	m_bOn = false;
	m_bMop = false;
	m_fCount = 0;
	m_fTimeAcc = 0.f;
}

void CLoading::TickStart(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTime < m_fTimeAcc)
	{
		if (m_fCount <= 0)
		{
			m_bMop = true;
			m_bRender = true;
			m_fCount = 0;
		}
		else
			--m_fCount;
		m_fTimeAcc = 0.f;
	}

}

void CLoading::TickEnd(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTime < m_fTimeAcc)
	{
		if (m_fCount > m_fMaxCount)
		{
			m_bRender = false;
		}
		else
			++m_fCount;
		m_fTimeAcc = 0.f;
	}
}

HRESULT CLoading::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CGameObject* pObj = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Loading_Mop"), &pObj, nullptr)))
		return E_FAIL;
	m_pMop = (CLoading_Mop*)pObj;


	Safe_Release(pGameInstance);

	return S_OK;
}


CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading*		pInstance = new CLoading(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading::Clone(void * pArg)
{
	CLoading*		pInstance = new CLoading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	__super::Free();

	Safe_Release(m_pMop);

}

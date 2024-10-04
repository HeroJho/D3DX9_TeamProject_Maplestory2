#include "stdafx.h"
#include "..\Public\EHelenaChat.h"

#include "GameInstance.h"
#include "TaxiButton.h"


CEHelenaChat::CEHelenaChat(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CEHelenaChat::CEHelenaChat(const CEHelenaChat& rhs)
	: CUI(rhs)
{
}





HRESULT CEHelenaChat::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CEHelenaChat::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = g_iWinSizeX * 0.8f;
	m_UIInfo.fSizeY = g_iWinSizeY * 0.6f;
	m_UIInfo.fX = g_iWinSizeX * 0.5f;
	m_UIInfo.fY = g_iWinSizeY - g_iWinSizeY * 0.3f;
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_iTexturenum = 0;

	__super::Initialize(pArg);



	m_bRender = true;


	D3DXCreateFont(m_pGraphic_Device, 29, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"), &m_pFont);
	D3DXCreateFont(m_pGraphic_Device, 32, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Name"), &m_pNameFont);



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);



	Safe_Release(pGameInstance);

	return S_OK;
}





void CEHelenaChat::Tick(_float fTimeDelta)
{

	if (m_bOn)
		On_Chat(fTimeDelta);
	else
		Off_Chat(fTimeDelta);

	if (CGameInstance::Get_Instance()->Key_Down(DIK_RETURN))
		End_Chat();

}

void CEHelenaChat::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CEHelenaChat::Render()
{
	if (!m_bRender)
		return S_OK;



	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_Texture(m_iCount)))
		return E_FAIL;

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();


	RECT rc;

	TCHAR cChat[128];




	if (m_bOn && 17 == m_iCount)
	{
		wsprintf(cChat, TEXT("헬레나"));
		SetRect(&rc, 260.f - 40.f, 500.f, 0, 0);
		m_pNameFont->DrawText(NULL, cChat,
			-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));


		wsprintf(cChat, TEXT("검은 마법사는 이쪽으로 도망갔습니다.\n나무가 무너지고 있으니 최대한 빨리 움직이세요. \n무운을 빌겠습니다."));
		SetRect(&rc, 260.f - 40.f, 535.f, 0, 0);
		m_pFont->DrawText(NULL, cChat,
			-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	}

	return S_OK;
}








void CEHelenaChat::Start_Chat()
{
	m_bOn = true;
	m_fTimeAcc = 0.f;
	m_iCount = 0;

	m_bRender = true;
}

void CEHelenaChat::End_Chat()
{
	m_bOn = false;
	m_fTimeAcc = 0.f;
	m_iCount = 17;
}

void CEHelenaChat::On_Chat(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.01f < m_fTimeAcc)
	{
		++m_iCount;
		if (17 < m_iCount)
		{
			m_iCount = 17;
		}

		m_fTimeAcc = 0.f;
	}
}

void CEHelenaChat::Off_Chat(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.01f < m_fTimeAcc)
	{
		--m_iCount;
		if (0 > m_iCount)
		{
			m_iCount = 0;
			m_bRender = false;
		}

		m_fTimeAcc = 0.f;
	}
}







HRESULT CEHelenaChat::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}







CEHelenaChat* CEHelenaChat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEHelenaChat*		pInstance = new CEHelenaChat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEHelenaChat::Clone(void * pArg)
{
	CEHelenaChat*		pInstance = new CEHelenaChat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CLoading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEHelenaChat::Free()
{
	__super::Free();


}

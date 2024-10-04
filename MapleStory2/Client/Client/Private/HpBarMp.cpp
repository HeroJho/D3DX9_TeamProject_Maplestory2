#include "stdafx.h"
#include "..\Public\HpBarMp.h"
#include "GameInstance.h"
#include "UIManager.h"

#include "ToolManager.h"
#include "ParticleManager.h"

CHpBarMp::CHpBarMp(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CHpBarMp::CHpBarMp(const CHpBarMp& rhs)
	: CUI(rhs)
{
}


HRESULT CHpBarMp::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CHpBarMp::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 150.f;
	m_UIInfo.fSizeY = 150.f;
	m_UIInfo.fX = g_iWinSizeX*0.5;
	m_UIInfo.fY = 600.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_iTexturenum = 0;
	__super::Initialize(pArg);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBarMp"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CHpBarMp::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	m_iTexturenum = CUIManager::Get_Instance()->Get_PlayerMp();

	if (pInstance->Key_Down(DIK_A))
	{
	
			CUIManager::Get_Instance()->Set_PlayerMp(1);
	}

	if (pInstance->Key_Down(DIK_D))
	{
		
			CUIManager::Get_Instance()->Set_PlayerMp(1);
	}

	if (pInstance->Key_Down(DIK_S))
	{
		
			CUIManager::Get_Instance()->Set_PlayerMp(1);
	}

	if (pInstance->Key_Down(DIK_F))
	{
	
			CUIManager::Get_Instance()->Set_PlayerMp(1);
	}

	if (pInstance->Key_Down(DIK_G))
	{
	
			CUIManager::Get_Instance()->Set_PlayerMp(2);
	}

	if (pInstance->Key_Down(DIK_Q))
	{
		
			CUIManager::Get_Instance()->Set_PlayerMp(5);
	}

	if (pInstance->Key_Down(DIK_C))
	{
			CUIManager::Get_Instance()->Set_PlayerMp(1);

	}
	
	if (pInstance->Key_Down(DIK_NUMPAD2))
	{
		pInstance->PlaySoundW(L"UseItem.mp3", 26, 1.f);

		CTransform* pTran = (CTransform*)CToolManager::Get_Instance()->GetPlayer()->Get_ComponentPtr(TEXT("Com_Transform"));
		_float3 vPos = pTran->Get_State(CTransform::STATE_POSITION);
		vPos.y += 1.5f;
		vPos.x -= 0.5f;
		CParticleManager::Get_Instance()->MakeDamageGen(1000, 1000, 1, 0.05f, vPos, 1.6f, true, CDamage::DAMAGE_HEAL);

		if (m_iTexturenum < 3)
			CUIManager::Get_Instance()->Full_PlayerMp();
		else
			CUIManager::Get_Instance()->Set_PlayerMp(-3);
	}


	Safe_Release(pInstance);
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);



	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);

}

void CHpBarMp::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CHpBarMp::Render()
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


CHpBarMp* CHpBarMp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHpBarMp*		pInstance = new CHpBarMp(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHpBarMp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHpBarMp::Clone(void * pArg)
{
	CHpBarMp*		pInstance = new CHpBarMp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHpBarMp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHpBarMp::Free()
{
	__super::Free();

}

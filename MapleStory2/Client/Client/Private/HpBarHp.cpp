#include "stdafx.h"
#include "..\Public\HpBarHp.h"
#include "GameInstance.h"
#include "UIManager.h"

#include "ParticleManager.h"
#include "ToolManager.h"


CHpBarHp::CHpBarHp(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CHpBarHp::CHpBarHp(const CHpBarHp& rhs)
	: CUI(rhs)
{
}


HRESULT CHpBarHp::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CHpBarHp::Initialize(void * pArg)
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBarHp"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CHpBarHp::Tick(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	m_iTexturenum = CUIManager::Get_Instance()->Get_PlayerHp();

	if (pInstance->Key_Down(DIK_NUMPAD1))
	{
		pInstance->PlaySoundW(L"UseItem.mp3", 26, 1.f);

		CTransform* pTran = (CTransform*)CToolManager::Get_Instance()->GetPlayer()->Get_ComponentPtr(TEXT("Com_Transform"));
		_float3 vPos = pTran->Get_State(CTransform::STATE_POSITION);
		vPos.y += 1.5f;
		vPos.x -= 0.5f;
		CParticleManager::Get_Instance()->MakeDamageGen(1000, 1000, 1, 0.05f, vPos, 1.6f, true, CDamage::DAMAGE_HEAL);

		if (m_iTexturenum < 2)
			CUIManager::Get_Instance()->Full_PlayerHp();
		else
			CUIManager::Get_Instance()->Set_PlayerHp(-3);
	}

	Safe_Release(pInstance);
	if (CUIManager::Get_Instance()->Get_StartMove())
		Start_CutScene(fTimeDelta);



	if (CUIManager::Get_Instance()->Get_EndMove())
		End_CutScene(fTimeDelta);

}

void CHpBarHp::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CHpBarHp::Render()
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


CHpBarHp* CHpBarHp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHpBarHp*		pInstance = new CHpBarHp(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHpBarHp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHpBarHp::Clone(void * pArg)
{
	CHpBarHp*		pInstance = new CHpBarHp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHpBarHp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHpBarHp::Free()
{
	__super::Free();

}

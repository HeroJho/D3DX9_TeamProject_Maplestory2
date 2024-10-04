#include "stdafx.h"
#include "..\Public\Taxi.h"

#include "GameInstance.h"

#include "CutSceneManager.h"
#include "UIManager.h"


CTaxi::CTaxi(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CTaxi::CTaxi(const CTaxi & rhs)
	: CCreature(rhs)
{
}




HRESULT CTaxi::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CTaxi::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Npc";

	m_fColRad = 1.f;	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(1.2f, 1.2f, -2.0f));
	m_pTransformCom->Set_Scaled(_float3(2.1f, 1.6f, 1.6f));



	return S_OK;
}




HRESULT CTaxi::SetUp_Components()
{
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Taxi"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CTaxi::Tick(_float fTimeDelta)
{



}
void CTaxi::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_UI, this);
}
HRESULT CTaxi::Render()
{
	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;


	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}





void CTaxi::Tick_Idle(_float fTimeDelta)
{
}
void CTaxi::Tick_Move(_float fTimeDelta)
{
}
void CTaxi::Tick_Hit(_float fTimeDelta)
{
}

void CTaxi::Tick_Chase(_float fTimeDelta)
{
}

CTaxi * CTaxi::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTaxi*		pInstance = new CTaxi(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTaxi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CTaxi::Clone(void* pArg)
{
	CTaxi*		pInstance = new CTaxi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTaxi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CTaxi::Collision(CGameObject * pOther)
{
	if ("Tag_Player" == pOther->Get_Tag())
	{
		if (CGameInstance::Get_Instance()->Key_Down(DIK_SPACE))
		{
			CCutSceneManager::Get_Instance()->Get_MainCam()->SetFocus(this, _float3{ 0.f, 1.f, -4.f }, 5.f);
			CUIManager::Get_Instance()->On_TaxiChatBox();
		}
	}
}




void CTaxi::Free()
{
	__super::Free();

}


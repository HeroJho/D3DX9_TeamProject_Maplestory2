#include "stdafx.h"
#include "..\Public\BeastBegin.h"
#include "BeastLoof.h"
#include "GameInstance.h"

CBeastBegin::CBeastBegin(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBeastBegin::CBeastBegin(const CBeastBegin & rhs)
	: CCreature(rhs), m_bCreate(false)
{
}




HRESULT CBeastBegin::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBeastBegin::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	Safe_Release(pInstance);

	m_pTransformCom->Set_Scaled(_float3{ 10.f,8.f,10.f });

	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Beast_Begin"), 0.1f, CAnimator::STATE_ONCE);
	memcpy(&m_Desc, pArg, sizeof(BEASTBEGINDESC));
	m_eDir = m_Desc.eDir;
	m_fYDistance = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	SetPosition(m_eDir);
	return S_OK;
}




HRESULT CBeastBegin::SetUp_Components()
{
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Beast_Begin"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}




void CBeastBegin::Tick(_float fTimeDelta)
{
	
}
void CBeastBegin::LateTick(_float fTimeDelta)
{

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);

	if (m_pAnimatorCom->Get_AnimCount() == 6 && !m_bCreate)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CBeastLoof::BEASTLOOFDESC LoofDesc;
		LoofDesc.eDir = m_eDir;
		LoofDesc.pTransform = m_pTransformCom;
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Beast_Loof"), LEVEL_STATIC, TEXT("Layer_Player_Skill"), &LoofDesc);
		Safe_Release(pGameInstance);
		m_bCreate = true;
		Set_Dead();
	}

	


}
HRESULT CBeastBegin::Render()
{
	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}


void CBeastBegin::SetState(STATE eState, DIR eDir)
{

}

void CBeastBegin::SetPosition(DIR eDir)
{
	_float3 vPosFix;
	switch (eDir)
	{
	case Client::CCreature::DIR_L:
		vPosFix = { -3.f,2.f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_R:
		vPosFix = { 3.f,2.f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_U:
		vPosFix = { 0.f,2.f,3.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_D:
		vPosFix = { 0.f,2.f,-3.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LU:
		vPosFix = { -3.f,2.f,3.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RU:
		vPosFix = { 3.f,2.f,3.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LD:
		vPosFix = { -3.f,2.f,-3.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RD:
		vPosFix = { 3.f,2.f,-3.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_END:

		break;
	default:
		break;
	}
}
void CBeastBegin::SetAni()
{

}



CBeastBegin * CBeastBegin::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBeastBegin*		pInstance = new CBeastBegin(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBeastBegin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBeastBegin::Clone(void* pArg)
{
	CBeastBegin*		pInstance = new CBeastBegin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBeastBegin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBeastBegin::Collision(CGameObject * pOther)
{

}

HRESULT CBeastBegin::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;

}

HRESULT CBeastBegin::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CBeastBegin::Free()
{
	__super::Free();
}


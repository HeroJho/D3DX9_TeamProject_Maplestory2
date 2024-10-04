#include "stdafx.h"
#include "..\Public\Potal.h"

#include "GameInstance.h"
#include "Player.h"
#include "Level_Loading.h"
#include "UIManager.h"
#include "ToolManager.h"

CPotal::CPotal(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CPotal::CPotal(const CPotal & rhs)
	: CCreature(rhs)
{
}




HRESULT CPotal::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CPotal::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Potal";

	POTALDESC* pPotalDesc = (POTALDESC*)pArg;
	_float3 vPos = pPotalDesc->Pos;
	m_vDestPos = pPotalDesc->DestPos;
	m_eDestLevel = pPotalDesc->eDestLevel;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y, vPos.z));

	m_fColRad = 0.9f;
	m_pTransformCom->Set_Scaled(_float3(1.5f, 3.0f, 1.5f));


	return S_OK;
}




HRESULT CPotal::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.5f, 1.f, 0.5f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Potal"), nullptr);
	}

	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Potal"), 0.05f, CAnimator::STATE_LOOF);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPotal::Set_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CPotal::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;
}




void CPotal::Tick(_float fTimeDelta)
{

	if (nullptr == m_pOther)
		return;

	m_fTimeAcc += fTimeDelta;

	if (5.f < m_fTimeAcc)
	{
		m_bTrigger = true;
	}




}
void CPotal::LateTick(_float fTimeDelta)
{
	__super::BoxColCom_Tick(m_pTransformCom);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_POTAL, this);

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);



	if (m_bTrigger)
	{
		CToolManager::Get_Instance()->SetDestLevel(m_eDestLevel, m_vDestPos);
	}

}
HRESULT CPotal::Render()
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

	// __super::BoxColCom_Render(m_pTransformCom);

	

	return S_OK;
}





CPotal * CPotal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPotal*		pInstance = new CPotal(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPotal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CPotal::Clone(void* pArg)
{
	CPotal*		pInstance = new CPotal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPotal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CPotal::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Player")
	{
		
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (pGameInstance->Key_Down(DIK_Z))
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"Portal.mp3", 29, 1.f);
			CUIManager::Get_Instance()->Start_Loading();
			
			m_pOther = pOther;
		}
	
		Safe_Release(pGameInstance);
	}
}




void CPotal::Free()
{
	__super::Free();

}


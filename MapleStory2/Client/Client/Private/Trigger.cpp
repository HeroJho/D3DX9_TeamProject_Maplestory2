#include "stdafx.h"
#include "..\Public\Trigger.h"

#include "GameInstance.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCube(pGraphic_Device)
{
}

CTrigger::CTrigger(const CTrigger & rhs)
	: CCube(rhs)
{
}


HRESULT CTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	m_sTag = "Tag_TriggerCube";

	m_pData = (CMap_Manager::CUBEDATA*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pData->vPos);

	// m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, 45.f);

	
	return S_OK;
}

void CTrigger::Tick(_float fTimeDelta)
{


	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (vPos.y < -10.f)
	{
		Set_Dead();
	}

}

void CTrigger::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Go_Gravity(fTimeDelta);
	

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float4x4		ViewMatrix, ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	D3DXVec3TransformCoord(&vPos, &vPos, &ViewMatrix);
	D3DXVec3TransformCoord(&vPos, &vPos, &ProjMatrix);

	_float3 LU = { -1.5f, 1.5f, 0.f };
	_float3 RD = { 1.5f, -1.5f, 0.f };

	if (LU.x < vPos.x && RD.x > vPos.x &&
		LU.y > vPos.y && RD.y < vPos.y)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_TRIGGERBLOCK, this);
	__super::BoxColCom_Tick(m_pTransformCom);
}

HRESULT CTrigger::Render()
{
	
	//__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}

HRESULT CTrigger::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	return S_OK;
}

HRESULT CTrigger::Reset_RenderState()
{


	return S_OK;
}

HRESULT CTrigger::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;



	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 2.f, 2.f, 2.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;



	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CTrigger * CTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrigger*		pInstance = new CTrigger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrigger::Clone(void* pArg)
{
	CTrigger*		pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	// Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


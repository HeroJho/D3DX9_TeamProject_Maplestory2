#include "stdafx.h"
#include "..\Public\Model_Die.h"

#include "GameInstance.h"
#include "ParticleManager.h"

CModel_Die::CModel_Die(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CModel_Die::CModel_Die(const CModel_Die & rhs)
	: CGameObject(rhs)
{
}


HRESULT CModel_Die::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModel_Die::Initialize(void * pArg)
{

	if (nullptr == pArg)
		return E_FAIL;


	CVIBuffer_Voxel::VOXELDESC* pVoxDesc = (CVIBuffer_Voxel::VOXELDESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pVoxDesc->vPos);
	m_pTransformCom->Set_Scaled(0.15f);
	//m_pTransformCom->RotationThree(
	//	_float3(1.f, 0.f, 0.f), pVoxDesc->vRotationAix.x,
	//	_float3(0.f, 1.f, 0.f), pVoxDesc->vRotationAix.y + 45.f,
	//	_float3(0.f, 0.f, 1.f), pVoxDesc->vRotationAix.z);

	return S_OK;
}

void CModel_Die::Tick(_float fTimeDelta)
{
	if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < -10)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3{-2.f, 2.f, -2.f});
		m_pTransformCom->Set_Vel(0.f);
	}

	m_iCount += fTimeDelta;
	if (m_iCount > 5.f)
		Set_Dead();
}

void CModel_Die::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLISION_ITEM, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CModel_Die::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel_Die::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	return S_OK;
}

HRESULT CModel_Die::Reset_RenderState()
{


	return S_OK;
}

HRESULT CModel_Die::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.5f, 0.5f, 0.5f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Model_Die"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}


void CModel_Die::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Cube")
	{
		int i = 0;
		if (!m_bLend)
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			CParticleManager::Get_Instance()->DieModel_Lend(vPos);
			m_bLend = true;
		}
	}
}



CModel_Die * CModel_Die::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CModel_Die*		pInstance = new CModel_Die(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CModel_Die::Clone(void* pArg)
{
	CModel_Die*		pInstance = new CModel_Die(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CWood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel_Die::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


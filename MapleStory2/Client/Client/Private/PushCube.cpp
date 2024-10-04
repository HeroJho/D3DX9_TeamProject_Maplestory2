#include "stdafx.h"
#include "..\Public\PushCube.h"

#include "GameInstance.h"
#include "ParticleManager.h"

CPushCube::CPushCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPushCube::CPushCube(const CPushCube & rhs)
	: CGameObject(rhs)
{
}


HRESULT CPushCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPushCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_sTag = "Tag_PushCube";

	CMap_Manager::CUBEDATA* desc = (CMap_Manager::CUBEDATA*)pArg;
	 m_pTransformCom->Set_State(CTransform::STATE_POSITION, desc->vPos);
	m_pTransformCom->Set_Scaled(0.15f);
	m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, 45.f);


	return S_OK;
}

void CPushCube::Tick(_float fTimeDelta)
{
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (vPos.y < -10.f)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-1.f, 1.f, -3.f));
		m_pTransformCom->Set_Vel(0.f);
	}
}

void CPushCube::LateTick(_float fTimeDelta)
{

	// if(CGameInstance::Get_Instance()->Key_Down(DIK_M))
		m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	//_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//_float4x4		ViewMatrix, ProjMatrix;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	//D3DXVec3TransformCoord(&vPos, &vPos, &ViewMatrix);
	//D3DXVec3TransformCoord(&vPos, &vPos, &ProjMatrix);

	//_float3 LU = { -1.5f, 1.5f, 0.f };
	//_float3 RD = { 1.5f, -1.5f, 0.f };

	//if (LU.x < vPos.x && RD.x > vPos.x &&
	//	LU.y > vPos.y && RD.y < vPos.y)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_PUSHBLOCK, this);
}

HRESULT CPushCube::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(Set_RenderState()))
		return E_FAIL;


	m_pVIBufferCom->Render();


	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	if (CGameInstance::Get_Instance()->Key_Down(DIK_0))
	{
		if (temp)
			temp = false;
		else
			temp = true;
	}

	if (temp)
		//__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}

void CPushCube::Collision(CGameObject * pOther)
{
	if ("Tag_GASMonster" == pOther->Get_Tag())
	{
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.y += 0.3f;
		CParticleManager::Get_Instance()->BlueMushRoom_Lend(vPos);
		pOther->Set_Dead();
		Set_Dead();
	}

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

HRESULT CPushCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	return S_OK;
}

HRESULT CPushCube::Reset_RenderState()
{


	return S_OK;
}

HRESULT CPushCube::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Model_Lock"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cube"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;




	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 1.0f, 1.0f, 1.0f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.3f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
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

CPushCube * CPushCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPushCube*		pInstance = new CPushCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPushCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPushCube::Clone(void* pArg)
{
	CPushCube*		pInstance = new CPushCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPushCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPushCube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


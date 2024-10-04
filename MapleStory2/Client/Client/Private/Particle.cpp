#include "stdafx.h"
#include "..\Public\Particle.h"

#include "GameInstance.h"



CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CParticle::CParticle(const CParticle & rhs)
	: CGameObject(rhs)
	, m_fLifeTimeAcc(0.f)
{
	ZeroMemory(&m_ParticleDesc, sizeof(PARTICLEDESC));
}


HRESULT CParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_ParticleDesc = *((PARTICLEDESC*)pArg);



	if (FAILED(SetUp_Components()))
		return E_FAIL;



	m_sTag = "Tag_Particle";



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_ParticleDesc.vPos);
	m_pTransformCom->Set_Scaled(m_ParticleDesc.fScale);
	m_pTransformCom->Set_Gravity(m_ParticleDesc.fGravityVal);


	m_pTransformCom->Set_Vel(m_ParticleDesc.fForce_Y);

	return S_OK;
}

void CParticle::Tick(_float fTimeDelta)
{
	m_fLifeTimeAcc += 1.f * fTimeDelta;

	if (m_ParticleDesc.fLifeTime < m_fLifeTimeAcc)
		Set_Dead();

}

void CParticle::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Go_DirForce(m_ParticleDesc.vDirVec, m_ParticleDesc.fForce_X, fTimeDelta);

	if(m_ParticleDesc.bGravity)
		m_pTransformCom->Go_Gravity(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CParticle::Render()
{
	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (nullptr != m_ParticleDesc.pTag)
	{
		if (FAILED(m_pTextureCom->Bind_Texture(0)))
			return E_FAIL;
	}
	else
		m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(Set_RenderState()))
		return E_FAIL;


	m_pVIBufferCom->Render();


	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CParticle::Reset_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);


	return S_OK;
}

HRESULT CParticle::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (nullptr != m_ParticleDesc.pTag)
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, m_ParticleDesc.pTag, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}



void CParticle::Set_Billboard()
{
	_float3 fScale = m_pTransformCom->Get_Scaled();

	m_vLookTemp = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * fScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * fScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * fScale.z);
}





CParticle * CParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticle*		pInstance = new CParticle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CParticle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParticle::Clone(void* pArg)
{
	CParticle*		pInstance = new CParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CParticle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


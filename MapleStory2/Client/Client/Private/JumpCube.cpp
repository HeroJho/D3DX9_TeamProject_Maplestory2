#include "stdafx.h"
#include "..\Public\JumpCube.h"

#include "GameInstance.h"
#include "Creature.h"
#include "ParticleManager.h"

CJumpCube::CJumpCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCube(pGraphic_Device)
{
}

CJumpCube::CJumpCube(const CJumpCube & rhs)
	: CCube(rhs)
{
}


HRESULT CJumpCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CJumpCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	m_sTag = "Tag_Cube";

	m_pData = *(CMap_Manager::CUBEDATA*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pData.vPos);

	m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, 45.f);

	//m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_BLOCK, this);
	//__super::BoxColCom_Tick(m_pTransformCom);

	return S_OK;
}

void CJumpCube::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (0.2f < m_fTimeAcc)
	{
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		CParticleManager::Get_Instance()->UpAir(vPos);

		m_fTimeAcc = 0.f;
	}
}

void CJumpCube::LateTick(_float fTimeDelta)
{
	__super::BoxColCom_Tick(m_pTransformCom);

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

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_DOWNBLOCK, this);
}

HRESULT CJumpCube::Render()
{
	return S_OK;


	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_pData.iIndex)))
		return E_FAIL;


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

	//if (temp)
	//	__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}

void CJumpCube::Collision(CGameObject * pOther)
{
	if ("Tag_Player" == pOther->Get_Tag())
	{
		CCreature* pCreature = (CCreature*)pOther;
		CTransform* pTran = (CTransform*)pCreature->Get_ComponentPtr(TEXT("Com_Transform"));


		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vOtherPos = pTran->Get_State(CTransform::STATE_POSITION);

		if (vPos.y + 0.5f < vOtherPos.y)
		{
			pCreature->Set_JumpPower(15.f);
			pCreature->SetState(CCreature::STATE_JUMP);

		}
	
	}
}


HRESULT CJumpCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	return S_OK;
}

HRESULT CJumpCube::Reset_RenderState()
{


	return S_OK;
}

HRESULT CJumpCube::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cube"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;




	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 1.0f, 1.0f, 1.0f };
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

CJumpCube * CJumpCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CJumpCube*		pInstance = new CJumpCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CJumpCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CJumpCube::Clone(void* pArg)
{
	CJumpCube*		pInstance = new CJumpCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CJumpCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CJumpCube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


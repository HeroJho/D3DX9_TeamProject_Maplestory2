#include "stdafx.h"
#include "..\Public\LaserBlockCube.h"

#include "GameInstance.h"
#include "ParticleManager.h"
#include "ToolManager.h"

CLaserBlockCube::CLaserBlockCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLaserBlockCube::CLaserBlockCube(const CLaserBlockCube & rhs)
	: CGameObject(rhs)
{
}


HRESULT CLaserBlockCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLaserBlockCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_sTag = "Tag_PushCube";

	CMap_Manager::CUBEDATA* desc = (CMap_Manager::CUBEDATA*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, desc->vPos);
	m_pTransformCom->Set_Scaled(0.15f);
	m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, 45.f);

	m_vOriPos = desc->vPos;

	return S_OK;
}

void CLaserBlockCube::Tick(_float fTimeDelta)
{
	CGameObject* pPlayer = CToolManager::Get_Instance()->GetPlayer();
	CTransform* pPTran = (CTransform*)pPlayer->Get_ComponentPtr(TEXT("Com_Transform"));
	_float3 vPPos = pPTran->Get_State(CTransform::STATE_POSITION);


	if (m_bPicked)
	{
		_float3 vPos = vPPos;
		vPos.y += 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		if (CGameInstance::Get_Instance()->Key_Down(DIK_Z))
		{
			m_bPicked = false;
			m_bDrop = true;
			m_pTransformCom->Set_Vel(2.f);

			_float3 vLook = pPTran->Get_State(CTransform::STATE_LOOK);
			D3DXVec3Normalize(&vLook, &vLook);
			m_vDir = vLook;
		}

	}
	else if (m_bDrop)
	{
		m_pTransformCom->Go_DirForce(m_vDir, 1.5f, fTimeDelta);
		CToolManager::Get_Instance()->Set_BlackCubeOn(false);
	}


	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (vPos.y < -10.f)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vOriPos);
		m_pTransformCom->Set_Vel(0.f);
	}
}

void CLaserBlockCube::LateTick(_float fTimeDelta)
{

	if (!m_bPicked)
		m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);


	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_PUSHBLOCK, this);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_PUSHBLOCK, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CLaserBlockCube::Render()
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

void CLaserBlockCube::Collision(CGameObject * pOther)
{
	if ("Tag_Monster" == pOther->Get_Tag())
	{
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.y += 0.3f;
		CParticleManager::Get_Instance()->BlueMushRoom_Lend(vPos);
		pOther->Set_Dead();
		if (m_bPicked)
			CToolManager::Get_Instance()->Set_BlackCubeOn(false);

		--m_iHp;
		if (1 > m_iHp)
		{
			Set_Dead();
			CToolManager::Get_Instance()->DecreaseBlackCubeCount();
		}
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
		m_pTransformCom->Set_Vel(1.f);
		m_bDrop = false;
	}

	if (pOther->Get_Tag() == "Tag_Player")
	{
		if (!CToolManager::Get_Instance()->Get_BlackCubeOn() && CGameInstance::Get_Instance()->Key_Down(DIK_Z))
		{
			m_bPicked = true;
			m_bLend = false;
			CToolManager::Get_Instance()->Set_BlackCubeOn(true);
		}
			
	}
}

HRESULT CLaserBlockCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	return S_OK;
}

HRESULT CLaserBlockCube::Reset_RenderState()
{


	return S_OK;
}

HRESULT CLaserBlockCube::SetUp_Components()
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
	BoxColDesc.vScale = _float3{ 0.5f, 1.0f, 0.5f };
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

CLaserBlockCube * CLaserBlockCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLaserBlockCube*		pInstance = new CLaserBlockCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPushCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLaserBlockCube::Clone(void* pArg)
{
	CLaserBlockCube*		pInstance = new CLaserBlockCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPushCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLaserBlockCube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


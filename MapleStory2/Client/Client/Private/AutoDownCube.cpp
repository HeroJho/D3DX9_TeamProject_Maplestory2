#include "stdafx.h"
#include "..\Public\AutoDownCube.h"

#include "GameInstance.h"

CAutoDownCube::CAutoDownCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCube(pGraphic_Device)
{
}

CAutoDownCube::CAutoDownCube(const CAutoDownCube & rhs)
	: CCube(rhs)
{
}


HRESULT CAutoDownCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAutoDownCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	m_sTag = "Tag_Cube";

	m_pData = (CMap_Manager::CUBEDATA*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pData->vPos);

	m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, 45.f);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_BLOCK, this);
	__super::BoxColCom_Tick(m_pTransformCom);

	return S_OK;
}

void CAutoDownCube::Tick(_float fTimeDelta)
{
	if (5 == m_pData->iIndex)
		return;

	m_fDownTimeAcc += fTimeDelta;

	
	_float fRanNum = CGameInstance::Get_Instance()->Get_FloatRandom(0.f, 3.f);
	_float  fSTime = 5.f + fRanNum;

	if (fSTime < m_fDownTimeAcc)
		m_bDown = true;


	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fDownTime = vPos.y + 5.f - fRanNum;

	if (m_bDown)
	{
		m_fTimeAcc += fTimeDelta;

		if (fDownTime + 3.f < m_fTimeAcc)
		{
			Down(fTimeDelta);
		}
		else if(fDownTime < m_fTimeAcc)
		{
			Shake(fTimeDelta);
		}
	}


	if (m_bDown && vPos.y < -10.f)
	{
		Set_Dead();
	}

}

void CAutoDownCube::LateTick(_float fTimeDelta)
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

}

HRESULT CAutoDownCube::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (5 == m_pData->iIndex)
	{
		if (FAILED(m_pTextureCom->Bind_Texture(m_pData->iIndex + 1)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_Texture(m_pData->iIndex)))
			return E_FAIL;
	}



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

void CAutoDownCube::Collision(CGameObject * pOther)
{

}

void CAutoDownCube::Shake(_float fTimeDelta)
{
	m_fShakeTimeAcc += fTimeDelta;
	if (0.05f < m_fShakeTimeAcc)
	{
		if (m_bShakeTurn)
			m_bShakeTurn = false;
		else
			m_bShakeTurn = true;
		m_fShakeTimeAcc = 0.f;
	}

	if (m_bShakeTurn)
		m_pTransformCom->RotationTwo(_float3{ 0.f, 1.f, 0.f }, 45.f, _float3{ 0.f, 0.f, 1.f }, 5.f);
	else
		m_pTransformCom->RotationTwo(_float3{ 0.f, 1.f, 0.f }, 45.f, _float3{ 0.f, 0.f, 1.f }, -5.f);

}

void CAutoDownCube::Down(_float fTimeDelta)
{
	m_pTransformCom->Go_Gravity(fTimeDelta);

	if (!m_bSound)
	{
		m_bSound = true;
	}
}

HRESULT CAutoDownCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	return S_OK;
}

HRESULT CAutoDownCube::Reset_RenderState()
{


	return S_OK;
}

HRESULT CAutoDownCube::SetUp_Components()
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

CAutoDownCube * CAutoDownCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAutoDownCube*		pInstance = new CAutoDownCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CAutoDownCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAutoDownCube::Clone(void* pArg)
{
	CAutoDownCube*		pInstance = new CAutoDownCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CAutoDownCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAutoDownCube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


#include "stdafx.h"
#include "..\Public\DownCube.h"

#include "GameInstance.h"
#include "ToolManager.h"

CDownCube::CDownCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCube(pGraphic_Device)
{
}

CDownCube::CDownCube(const CDownCube & rhs)
	: CCube(rhs)
{
}


HRESULT CDownCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDownCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	m_sTag = "Tag_Cube";

	m_pData = *(CMap_Manager::CUBEDATA*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pData.vPos);

	m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, 45.f);

	return S_OK;
}

void CDownCube::Tick(_float fTimeDelta)
{
	if ((3 == m_pData.iIndex) && CToolManager::Get_Instance()->Get_MiniEnd())
	{
		m_bDown = true;
		CGameInstance::Get_Instance()->PlaySoundW(L"DesBlock.mp3", 4, 1.f);
	}
	else if ((4 == m_pData.iIndex) && (0 == CToolManager::Get_Instance()->GetMonCount()))
	{
		m_bDown = true;
		CGameInstance::Get_Instance()->PlaySoundW(L"DesBlock.mp3", 4, 1.f);
	}



	if (m_bDown)
	{
		m_fTimeAcc += fTimeDelta;

		if (1.f < m_fTimeAcc)
		{
			Down(fTimeDelta);
		}
		else
		{
			Shake(fTimeDelta);
		}
	}



	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (m_bDown && vPos.y < -8.f)
	{
		Set_Dead();
	}

	if (!(3 == m_pData.iIndex) && !(4 == m_pData.iIndex))
	{
		CGameObject* pPlayer = CToolManager::Get_Instance()->GetPlayer();
		CTransform* pPTran = (CTransform*)pPlayer->Get_ComponentPtr(TEXT("Com_Transform"));
		_float3 vPPos = pPTran->Get_State(CTransform::STATE_POSITION);

		if (vPPos.y < -10.f)
		{
			Set_Dead();
		}
	}
}

void CDownCube::LateTick(_float fTimeDelta)
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

HRESULT CDownCube::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_pData.iIndex)))
		return E_FAIL;


	if (FAILED(Set_RenderState()))
		return E_FAIL;


	m_pVIBufferCom->Render();


	if (FAILED(Reset_RenderState()))
		return E_FAIL;


	//if (temp)
	//__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}

void CDownCube::Collision(CGameObject * pOther)
{
	if (("Tag_Player" == pOther->Get_Tag()) && !(3 == m_pData.iIndex) && !(4 == m_pData.iIndex) && !m_bDown)
	{
		m_bDown = true;
		CGameInstance::Get_Instance()->PlaySoundW(L"DesBlock.mp3", 4, 1.f);
	}
}

void CDownCube::Shake(_float fTimeDelta)
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

void CDownCube::Down(_float fTimeDelta)
{
	m_pTransformCom->Go_Gravity(fTimeDelta);
}

HRESULT CDownCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	return S_OK;
}

HRESULT CDownCube::Reset_RenderState()
{


	return S_OK;
}

HRESULT CDownCube::SetUp_Components()
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
	BoxColDesc.vScale = _float3{ 0.4f, 1.0f, 0.4f };
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

CDownCube * CDownCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDownCube*		pInstance = new CDownCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDownCube::Clone(void* pArg)
{
	CDownCube*		pInstance = new CDownCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDownCube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


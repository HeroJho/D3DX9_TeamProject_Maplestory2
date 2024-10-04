#include "stdafx.h"
#include "..\Public\SeeCube.h"

#include "GameInstance.h"
#include "Shader.h"

CSeeCube::CSeeCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSeeCube::CSeeCube(const CSeeCube & rhs)
	: CGameObject(rhs)
{
}


HRESULT CSeeCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSeeCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	m_sTag = "Tag_Cube";

	m_pData = (CMap_Manager::CUBEDATA*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pData->vPos);

	m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, 45.f);


	if (6 == m_pData->iIndex)
	{
		m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_BLOCK, this);
		__super::BoxColCom_Tick(m_pTransformCom);
	}


	return S_OK;
}

void CSeeCube::Tick(_float fTimeDelta)
{

}

void CSeeCube::LateTick(_float fTimeDelta)
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

HRESULT CSeeCube::Render()
{
	/*if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
	return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_pData->iIndex)))
	return E_FAIL;


	if (FAILED(Set_RenderState()))
	return E_FAIL;


	m_pVIBufferCom->Render();


	if (FAILED(Reset_RenderState()))
	return E_FAIL;*/

	_float4x4			WorldMatrix, ViewMatrix, ProjMatrix;
	_float4x4			ViewMatrixInv;
	_float4x4			PlayerWorld;

	PlayerWorld = ((CTransform*)CGameInstance::Get_Instance()->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0))->Get_WorldMatrix();

	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->Set_RawValue("g_vPlayerPosition", &PlayerWorld.m[3][0], sizeof(_float4));

	m_pShaderCom->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	//m_pShaderCom->Set_RawValue("g_vCamPosition", &ViewMatrixInv.m[3][0], sizeof(_float4));

	m_pShaderCom->Set_Texture("g_Texture", m_pTextureCom->Get_Tex(m_pData->iIndex));

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->End();

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

HRESULT CSeeCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CSeeCube::Reset_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CSeeCube::SetUp_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.5f, 1.0f, 0.5f };
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
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CSeeCube * CSeeCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSeeCube*		pInstance = new CSeeCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSeeCube::Clone(void* pArg)
{
	CSeeCube*		pInstance = new CSeeCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSeeCube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
}

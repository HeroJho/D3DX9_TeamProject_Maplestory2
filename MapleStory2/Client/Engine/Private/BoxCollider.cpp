#include "..\Public\BoxCollider.h"

#include "GameObject.h"
#include "Transform.h"

CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}
CBoxCollider::CBoxCollider(const CBoxCollider & rhs)
	: CComponent(rhs)
	, m_vMax(rhs.m_vMax)
	, m_vMin(rhs.m_vMin)
{

}

_float CBoxCollider::Get_MaxRad()
{
	_float3 fNomalMax;
	D3DXVec3Normalize(&fNomalMax, &m_vMax);
	_float fRad = D3DXVec3Dot(&fNomalMax, &_float3(0.f, 1.f, 0.f));

	return fRad;
}

HRESULT CBoxCollider::Initialize_Prototype()
{
	m_vMax = { 0.5f, 0.5f, 0.5f };
	m_vMin = { -0.5f, -0.5f, -0.5f };

	return S_OK;
}
HRESULT CBoxCollider::Initialize(void * pArg)
{

	if (nullptr == pArg)
		return S_FALSE;

	{
		if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXCUBETEX) * 8, 0, D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0), D3DPOOL_MANAGED, &m_pVB, nullptr)))
			return E_FAIL;
		if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(FACEINDICES16) * 16, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
			return E_FAIL;

		VTXCUBETEX*		pVertices = nullptr;

		m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

		pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
		pVertices[0].vTexture = pVertices[0].vPosition;
		pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
		pVertices[1].vTexture = pVertices[1].vPosition;
		pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
		pVertices[2].vTexture = pVertices[2].vPosition;
		pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
		pVertices[3].vTexture = pVertices[3].vPosition;

		pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
		pVertices[4].vTexture = pVertices[4].vPosition;
		pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
		pVertices[5].vTexture = pVertices[5].vPosition;
		pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
		pVertices[6].vTexture = pVertices[6].vPosition;
		pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
		pVertices[7].vTexture = pVertices[7].vPosition;


		m_pVB->Unlock();

		FACEINDICES16*		pIndices = nullptr;

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		/* +x */
		pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
		pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

		/* -x */
		pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
		pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

		/* +y */
		pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
		pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

		/* -y */
		pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
		pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

		/* +z */
		pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
		pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

		/* -z */
		pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
		pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;


		m_pIB->Unlock();
	}
	

	m_Desc = *(BOXCOLCOMEDESC*)pArg;

	m_vMax.x *= m_Desc.vScale.x;
	m_vMax.y *= m_Desc.vScale.y;
	m_vMax.z *= m_Desc.vScale.z;
				
	m_vMin.x *= m_Desc.vScale.x;
	m_vMin.y *= m_Desc.vScale.y;
	m_vMin.z *= m_Desc.vScale.z;


	m_vMax.x += m_Desc.vPivot.x;
	m_vMax.y += m_Desc.vPivot.y;
	m_vMax.z += m_Desc.vPivot.z;

	m_vMin.x += m_Desc.vPivot.x;
	m_vMin.y += m_Desc.vPivot.y;
	m_vMin.z += m_Desc.vPivot.z;

	return S_OK;
}

void CBoxCollider::Tick(CTransform* pTrans)
{
	// 1.스케일을 보관
	_float3 vScale = pTrans->Get_Scaled();

	// 2.방향 1 1 1 * 스케일 짜리 포스있는 것만
	_float4x4 BoxMat;
	D3DXMatrixIdentity(&BoxMat);
	*(_float3*)(&BoxMat.m[0][0]) = _float3{ 1.f, 0.f, 0.f };
	*(_float3*)(&BoxMat.m[1][0]) = _float3{ 0.f, 1.f, 0.f };
	*(_float3*)(&BoxMat.m[2][0]) = _float3{ 0.f, 0.f, 1.f };
	*(_float3*)(&BoxMat.m[3][0]) = (pTrans->Get_State(CTransform::STATE_POSITION));

	// 3.vMax, vMin 월드를 구한다.
	D3DXVec3TransformCoord(&m_vWorldMax, &m_vMax, &BoxMat);
	D3DXVec3TransformCoord(&m_vWorldMin, &m_vMin, &BoxMat);
}

void CBoxCollider::Render(CTransform* pTrans)
{
	_float3 vScale = pTrans->Get_Scaled();

	_float4x4 WorldMatrix;
	D3DXMatrixIdentity(&WorldMatrix);
	*(_float3*)(&WorldMatrix.m[0][0]) = _float3{ 1.f * m_Desc.vScale.x, 0.f, 0.f };
	*(_float3*)(&WorldMatrix.m[1][0]) = _float3{ 0.f, 1.f * m_Desc.vScale.y, 0.f };
	*(_float3*)(&WorldMatrix.m[2][0]) = _float3{ 0.f, 0.f, 1.f * m_Desc.vScale.z };
	*(_float3*)(&WorldMatrix.m[3][0]) = (pTrans->Get_State(CTransform::STATE_POSITION));

	((_float3*)(&WorldMatrix.m[3][0]))->x += m_Desc.vPivot.x;
	((_float3*)(&WorldMatrix.m[3][0]))->y += m_Desc.vPivot.y;
	((_float3*)(&WorldMatrix.m[3][0]))->z += m_Desc.vPivot.z;

	

	m_pGraphic_Device->SetTexture(0, nullptr);

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBETEX));

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0));

	 //m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 16);

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}










CBoxCollider * CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoxCollider*			pInstance = new CBoxCollider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBoxCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CComponent * CBoxCollider::Clone(void * pArg)
{
	CBoxCollider*			pInstance = new CBoxCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBoxCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CBoxCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	__super::Free();
}

#include "..\Public\VIBuffer_ColorRect.h"
#include "Picking.h"
#include "Transform.h"

CVIBuffer_ColorRect::CVIBuffer_ColorRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_ColorRect::CVIBuffer_ColorRect(const CVIBuffer_ColorRect & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_ColorRect::Initialize_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pVerticesPos = new _float3[m_iNumVertices];

	VTXTEX*		pVertices = nullptr;


	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	m_pVerticesPos[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	m_pVerticesPos[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	m_pVerticesPos[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexture = _float2(1.f, 1.f);	

	m_pVerticesPos[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexture = _float2(0.f, 1.f);

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;
	
	m_pIB->Unlock();
	
	return S_OK;
}

HRESULT CVIBuffer_ColorRect::Initialize(void * pArg)
{
	return S_OK;
}

bool CVIBuffer_ColorRect::Picking(CTransform * pTransform, _float3 * pOut)
{
	CPicking*		pPicking = CPicking::Get_Instance();

	Safe_AddRef(pPicking);

	_float4x4		WorldMatrixInv = pTransform->Get_WorldMatrixInverse();

	_float3			vRayDir, vRayPos;

	pPicking->Compute_LocalRayInfo(&vRayDir, &vRayPos, pTransform);

	D3DXVec3Normalize(&vRayDir, &vRayDir);


	_float		fU, fV, fDist;
	_float4x4	WorldMatrix = pTransform->Get_WorldMatrix();

	/* 오른쪽 상단. */
	if (true == D3DXIntersectTri(&m_pVerticesPos[0], &m_pVerticesPos[1], &m_pVerticesPos[2], &vRayPos, &vRayDir, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayDir * fDist;

		D3DXVec3TransformCoord(pOut, &vPickPos, &WorldMatrix);

		Safe_Release(pPicking);
		return true;
	}

	/* 왼쪽 하단. */
	if (true == D3DXIntersectTri(&m_pVerticesPos[0], &m_pVerticesPos[2], &m_pVerticesPos[3], &vRayPos, &vRayDir, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayDir * fDist;

		D3DXVec3TransformCoord(pOut, &vPickPos, &WorldMatrix);

		Safe_Release(pPicking);
		return true;
	}


	Safe_Release(pPicking);

	return false;
}


CVIBuffer_ColorRect * CVIBuffer_ColorRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_ColorRect*			pInstance = new CVIBuffer_ColorRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_ColorRect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_ColorRect::Clone(void * pArg)
{
	CVIBuffer_ColorRect*			pInstance = new CVIBuffer_ColorRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_ColorRect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_ColorRect::Free()
{
	__super::Free();
}

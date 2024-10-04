#include "..\Public\VIBuffer_Cube.h"

#include "Transform.h"
#include "GameObject.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
#pragma region 중략
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0)/*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 16;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXCUBETEX*		pVertices = nullptr;

	m_pVerticesPos = new _float3[m_iNumVertices];
#pragma endregion
	// 정점 버퍼 생성
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	m_pVerticesPos[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexture = pVertices[0].vPosition; // UV 정보가 아니라 벡터 정보
	m_pVerticesPos[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexture = pVertices[1].vPosition;
	m_pVerticesPos[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexture = pVertices[2].vPosition;
	m_pVerticesPos[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexture = pVertices[3].vPosition;

	m_pVerticesPos[4] = pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexture = pVertices[4].vPosition;
	m_pVerticesPos[5] = pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexture = pVertices[5].vPosition;
	m_pVerticesPos[6] = pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexture = pVertices[6].vPosition;
	m_pVerticesPos[7] = pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexture = pVertices[7].vPosition;

	m_pVB->Unlock();
#pragma region 중략
	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16*		pIndices = nullptr;
#pragma endregion
	// 인덱스 버퍼 생성
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
	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void * pArg)
{
	return S_OK;
}




_bool CVIBuffer_Cube::PickCube(CGameObject* pMe, CGameObject *pOther, _float3* pOutDis, _float3* pOutVec)
{

	CTransform*		pMeTransform		= (CTransform*)pMe->Get_ComponentPtr(TEXT("Com_Transform"));
	_float4x4		Me_WorldMatrixInv	= pMeTransform->Get_WorldMatrixInverse();

	CTransform*		pOtherTransform = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));


	_float3			vRayLook, vRayDown, vRayPos;
 
	vRayPos = pOtherTransform->Get_State(CTransform::STATE_POSITION);
	// vRayLook = pTransform->Get_State(CTransform::STATE_LOOK);
	vRayLook = _float3{ 0.f, -1.f, 0.f };


	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &Me_WorldMatrixInv);
	// D3DXVec3TransformNormal(&vRayLook, &vRayLook, &Me_WorldMatrixInv);
	D3DXVec3Normalize(&vRayLook, &vRayLook);


	_float		fU, fV, fDist;

	_float fMinDis = 9999999.f;
	_float3	vMinPickPickVec;
	_float3 vMinPickPos;

	if (true == D3DXIntersectTri(&m_pVerticesPos[1], &m_pVerticesPos[5], &m_pVerticesPos[6], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[1], &m_pVerticesPos[5], &m_pVerticesPos[6]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[1], &m_pVerticesPos[6], &m_pVerticesPos[2], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[1], &m_pVerticesPos[6], &m_pVerticesPos[2]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[0], &m_pVerticesPos[3], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[0], &m_pVerticesPos[3]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[3], &m_pVerticesPos[7], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[3], &m_pVerticesPos[7]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[5], &m_pVerticesPos[1], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[5], &m_pVerticesPos[1]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[1], &m_pVerticesPos[0], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[1], &m_pVerticesPos[0]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[3], &m_pVerticesPos[2], &m_pVerticesPos[6], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[3], &m_pVerticesPos[2], &m_pVerticesPos[6]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[3], &m_pVerticesPos[6], &m_pVerticesPos[7], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[3], &m_pVerticesPos[6], &m_pVerticesPos[7]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[5], &m_pVerticesPos[4], &m_pVerticesPos[7], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[5], &m_pVerticesPos[4], &m_pVerticesPos[7]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[5], &m_pVerticesPos[7], &m_pVerticesPos[6], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[5], &m_pVerticesPos[7], &m_pVerticesPos[6]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[0], &m_pVerticesPos[1], &m_pVerticesPos[2], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[0], &m_pVerticesPos[1], &m_pVerticesPos[2]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[0], &m_pVerticesPos[2], &m_pVerticesPos[3], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[0], &m_pVerticesPos[2], &m_pVerticesPos[3]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (9999999.f != fMinDis)
	{

		_float		fU, fV, fDist;
		_float4x4	WorldMatrix = pMeTransform->Get_WorldMatrix();

		*pOutVec = vMinPickPickVec;
		D3DXVec3TransformCoord(pOutDis, &vMinPickPos, &WorldMatrix);
		return true;
	}


	return false;

}

_bool CVIBuffer_Cube::PickLookCube(CGameObject * pMe, CGameObject * pOther, _float3 * pOutDis, _float3 * pOutVec)
{

	CTransform*		pMeTransform = (CTransform*)pMe->Get_ComponentPtr(TEXT("Com_Transform"));
	_float4x4		Me_WorldMatrixInv = pMeTransform->Get_WorldMatrixInverse();

	CTransform*		pOtherTransform = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));


	_float3			vRayLook, vRayDown, vRayPos;

	vRayPos = pOtherTransform->Get_State(CTransform::STATE_POSITION);
	vRayLook = pOtherTransform->Get_State(CTransform::STATE_LOOK);
	// vRayLook = _float3{ 0.f, -1.f, 0.f };


	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &Me_WorldMatrixInv);
	// D3DXVec3TransformNormal(&vRayLook, &vRayLook, &Me_WorldMatrixInv);
	D3DXVec3Normalize(&vRayLook, &vRayLook);


	_float		fU, fV, fDist;

	_float fMinDis = 9999999.f;
	_float3	vMinPickPickVec;
	_float3 vMinPickPos;

	if (true == D3DXIntersectTri(&m_pVerticesPos[1], &m_pVerticesPos[5], &m_pVerticesPos[6], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[1], &m_pVerticesPos[5], &m_pVerticesPos[6]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[1], &m_pVerticesPos[6], &m_pVerticesPos[2], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[1], &m_pVerticesPos[6], &m_pVerticesPos[2]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[0], &m_pVerticesPos[3], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[0], &m_pVerticesPos[3]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[3], &m_pVerticesPos[7], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[3], &m_pVerticesPos[7]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[5], &m_pVerticesPos[1], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[5], &m_pVerticesPos[1]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[4], &m_pVerticesPos[1], &m_pVerticesPos[0], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[4], &m_pVerticesPos[1], &m_pVerticesPos[0]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[3], &m_pVerticesPos[2], &m_pVerticesPos[6], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[3], &m_pVerticesPos[2], &m_pVerticesPos[6]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[3], &m_pVerticesPos[6], &m_pVerticesPos[7], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[3], &m_pVerticesPos[6], &m_pVerticesPos[7]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[5], &m_pVerticesPos[4], &m_pVerticesPos[7], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[5], &m_pVerticesPos[4], &m_pVerticesPos[7]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[5], &m_pVerticesPos[7], &m_pVerticesPos[6], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[5], &m_pVerticesPos[7], &m_pVerticesPos[6]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (true == D3DXIntersectTri(&m_pVerticesPos[0], &m_pVerticesPos[1], &m_pVerticesPos[2], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[0], &m_pVerticesPos[1], &m_pVerticesPos[2]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}
	if (true == D3DXIntersectTri(&m_pVerticesPos[0], &m_pVerticesPos[2], &m_pVerticesPos[3], &vRayPos, &vRayLook, &fU, &fV, &fDist))
	{
		_float3	vPickPos = vRayPos + vRayLook * fDist;

		D3DXPLANE			Plane;
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[0], &m_pVerticesPos[2], &m_pVerticesPos[3]);
		_float3 vPickVec{ Plane.a, Plane.b, Plane.c };
		D3DXVec3Normalize(&vPickVec, &vPickVec);

		if (fDist < fMinDis)
		{
			fMinDis = fDist;
			vMinPickPos = vPickPos;
			vMinPickPickVec = vPickVec;
		}
		//return true;
	}


	if (9999999.f != fMinDis)
	{

		_float		fU, fV, fDist;
		_float4x4	WorldMatrix = pMeTransform->Get_WorldMatrix();

		*pOutVec = vMinPickPickVec;
		D3DXVec3TransformCoord(pOutDis, &vMinPickPos, &WorldMatrix);
		return true;
	}


	return false;

}







CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube*			pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone(void * pArg)
{
	CVIBuffer_Cube*			pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}

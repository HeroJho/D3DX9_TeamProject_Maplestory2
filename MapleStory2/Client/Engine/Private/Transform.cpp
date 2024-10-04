#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
{

}

void CTransform::Set_Scaled(_float3 vScale)
{
	m_vOriginScale = _float3{ vScale.x, vScale.y, vScale.z };

	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);

}

void CTransform::Set_Scaled(_float fScale)
{
	m_vOriginScale = _float3{ fScale, fScale, fScale };

	_float3 fR = Get_State(CTransform::STATE_RIGHT);
	_float3 fU = Get_State(CTransform::STATE_UP);
	_float3 fL = Get_State(CTransform::STATE_LOOK);

	D3DXVec3Normalize(&fR, &fR);
	D3DXVec3Normalize(&fU, &fU);
	D3DXVec3Normalize(&fL, &fL);

	fR *= fScale;
	fU *= fScale;
	fL *= fScale;

	Set_State(CTransform::STATE_RIGHT, fR);
	Set_State(CTransform::STATE_UP, fU);
	Set_State(CTransform::STATE_LOOK, fL);
}

void CTransform::Set_ScaledX(_float fScale)
{
	_float3 fR = Get_State(CTransform::STATE_RIGHT);
	fR *= fScale;
	Set_State(CTransform::STATE_RIGHT, fR);
}

_float3 CTransform::Get_Scaled() const
{
	return _float3(D3DXVec3Length(&Get_State(CTransform::STATE_RIGHT)), 
		D3DXVec3Length(&Get_State(CTransform::STATE_UP)), 
		D3DXVec3Length(&Get_State(CTransform::STATE_LOOK)));
}

HRESULT CTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::Bind_WorldMatrix()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vUp= Get_State(CTransform::STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Dir(_float3 vDir, _float fSpeed, _float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	D3DXVec3Normalize(&vDir, &vDir);

	vPosition += vDir * fSpeed * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}






void CTransform::Go_U(_float fTimeDelta)
{
	// _float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(0.f, 0.f, 1.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}
void CTransform::Go_D(_float fTimeDelta)
{
	_float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(0.f, 0.f, -1.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}
void CTransform::Go_L(_float fTimeDelta)
{
	_float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(-1.f, 0.f, 0.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}
void CTransform::Go_R(_float fTimeDelta)
{
	_float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(1.f, 0.f, 0.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}
void CTransform::Go_LU(_float fTimeDelta)
{
	_float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(-1.f, 0.f, 1.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}
void CTransform::Go_RU(_float fTimeDelta)
{
	_float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(1.f, 0.f, 1.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}
void CTransform::Go_LD(_float fTimeDelta)
{
	_float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(-1.f, 0.f, -1.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}
void CTransform::Go_RD(_float fTimeDelta)
{
	_float3 vTempPos = Get_State(STATE_LOOK);

	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = _float3(1.f, 0.f, -1.f);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	CulRUByLook(vLook);
}




void CTransform::Go_DirForce(_float3 vDirVec, _float fForce, _float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vNomaledDirVec = *D3DXVec3Normalize(&vDirVec, &vDirVec);
	CulRUByLook(vNomaledDirVec);

	//m_fTimeAcc += fTimeDelta;
	//m_fVel_Y -= m_fGravity * m_fTimeAcc;
	//vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_fVel_Y * fTimeDelta;

	vPosition += vNomaledDirVec * fForce * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Gravity(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	m_fTimeAcc += fTimeDelta;

	m_fVel_Y -= m_fGravity * m_fTimeAcc;

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_fVel_Y * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	// CulRUByLook(vLook);
}





void CTransform::Rotation(_float3 vAxis, _float fRadian)
{

	_float3		vScale = Get_Scaled();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScale.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScale.z;

	_float4x4	RotationMatrix;
	

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fRadian));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

}

void CTransform::RotationTwo(_float3 vAxis, _float fRadian, _float3 vAxis2, _float fRadian2)
{
	_float3		vScale = Get_Scaled();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScale.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScale.z;

	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fRadian));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	_float3		vRight2 = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp2 = Get_State(CTransform::STATE_UP);
	_float3		vLook2 = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix2;

	D3DXMatrixRotationAxis(&RotationMatrix2, &vAxis2, D3DXToRadian(fRadian2));

	D3DXVec3TransformNormal(&vRight2, &vRight2, &RotationMatrix2);
	D3DXVec3TransformNormal(&vUp2, &vUp2, &RotationMatrix2);
	D3DXVec3TransformNormal(&vLook2, &vLook2, &RotationMatrix2);

	Set_State(CTransform::STATE_RIGHT, vRight2);
	Set_State(CTransform::STATE_UP, vUp2);
	Set_State(CTransform::STATE_LOOK, vLook2);
}

void CTransform::RotationThree(_float3 vAxis, _float fRadian, _float3 vAxis2, _float fRadian2, _float3 vAxis3, _float fRadian3)
{
	_float3		vScale = Get_Scaled();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScale.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScale.z;

	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fRadian));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);


	_float3		vRight2 = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp2 = Get_State(CTransform::STATE_UP);
	_float3		vLook2 = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix2;

	D3DXMatrixRotationAxis(&RotationMatrix2, &vAxis2, D3DXToRadian(fRadian2));

	D3DXVec3TransformNormal(&vRight2, &vRight2, &RotationMatrix2);
	D3DXVec3TransformNormal(&vUp2, &vUp2, &RotationMatrix2);
	D3DXVec3TransformNormal(&vLook2, &vLook2, &RotationMatrix2);

	Set_State(CTransform::STATE_RIGHT, vRight2);
	Set_State(CTransform::STATE_UP, vUp2);
	Set_State(CTransform::STATE_LOOK, vLook2);


	_float3		vRight3 = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp3 = Get_State(CTransform::STATE_UP);
	_float3		vLook3 = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix3;

	D3DXMatrixRotationAxis(&RotationMatrix3, &vAxis3, D3DXToRadian(fRadian3));

	D3DXVec3TransformNormal(&vRight3, &vRight3, &RotationMatrix3);
	D3DXVec3TransformNormal(&vUp3, &vUp3, &RotationMatrix3);
	D3DXVec3TransformNormal(&vLook3, &vLook3, &RotationMatrix3);

	Set_State(CTransform::STATE_RIGHT, vRight3);
	Set_State(CTransform::STATE_UP, vUp3);
	Set_State(CTransform::STATE_LOOK, vLook3);
}


void CTransform::Turn(_float3 vAxis, _float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::RotationSpot(_float3 vAxisPos, _float3 vDisVec, _float fAngle)
{
	// 1. 플레이어 0 기준 카메라 위치


	// 2. 회전행렬을 만들어 곱한다
	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fAngle));
	D3DXVec3TransformCoord(&vDisVec, &vDisVec, &RotationMatrix);

	// 3. 포스를 플레이어 포스에 더한다.
	_float3 fDestPos = vAxisPos + vDisVec;

	Set_State(STATE_POSITION, fDestPos);
	LookAt(vAxisPos);
}


void CTransform::LookAt(_float3 vTargetPos)
{
	_float3		vScale = Get_Scaled();
	
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3	vLook = vTargetPos - vPosition;

	_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	_float3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::LookAtForLandObject(_float3 vTargetPos)
{
	_float3		vScale = Get_Scaled();

	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3	vLook = vTargetPos - vPosition;

	_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	_float3 vUp = _float3(0.f, 1.f, 0.f);

	vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);



	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z );
}

void CTransform::Chase(_float3 vTargetPos, _float fTimeDelta)
{
	LookAtForLandObject(vTargetPos);

	Go_Straight(fTimeDelta);
}

void CTransform::CulRUByLook(_float3 vLook)
{
	_float3		vScale = m_vOriginScale;

	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	_float3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*			pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*			pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}

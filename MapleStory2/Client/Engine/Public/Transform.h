#pragma once

#include "Component.h"

/* 1.객체의 월드상태를 보관한다.(행렬) */
/* 2.로컬상태의 정보를 월드상태로 변환해주기위한기능  */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATETYPE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	}TRANSFORMDESC;
public:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public: 
	_float3 Get_State(STATETYPE eState) const {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	void Set_Scaled(_float3 vScale);
	void Set_Scaled(_float fScale);
	void Set_ScaledX(_float fScale);
	_float3 Get_Scaled() const;


	_float4x4 Get_WorldMatrixInverse() {
		_float4x4		WorldMatrixInv;
		return	*D3DXMatrixInverse(&WorldMatrixInv, nullptr, &m_WorldMatrix);
	}

	_float4x4 Get_WorldMatrix() {
		return m_WorldMatrix;
	}

	void Set_State(STATETYPE eState, const _float3& State) {
		memcpy(&m_WorldMatrix.m[eState][0], &State, sizeof(_float3));		
	}


	void Set_Vel(_float fVel) { m_fVel_Y = fVel; m_fTimeAcc = 0.f; }
	void Plus_Vel(_float fVel) { m_fVel_Y += fVel; m_fTimeAcc = 0.f; }

	_float Get_Vel() { return m_fVel_Y; }
	void Set_Gravity(_float fGravity) { m_fGravity = fGravity; }
	void ReSet_Gravity() { m_fVel_Y = 0.f; m_fTimeAcc = 0.f; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_WorldMatrix();

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Dir(_float3 vDir, _float fSpeed, _float fTimeDelta);

	void Go_U(_float fTimeDelta);
	void Go_D(_float fTimeDelta);
	void Go_L(_float fTimeDelta);
	void Go_R(_float fTimeDelta);
	void Go_LU(_float fTimeDelta);
	void Go_RU(_float fTimeDelta);
	void Go_LD(_float fTimeDelta);
	void Go_RD(_float fTimeDelta);

	void Go_DirForce(_float3 vDirVec, _float fForce, _float fTimeDelta);
	void Go_Gravity(_float fTimeDelta);

	void Rotation(_float3 vAxis, _float fRadian);
	void RotationTwo(_float3 vAxis, _float fRadian, _float3 vAxis2, _float fRadian2);
	void RotationThree(_float3 vAxis, _float fRadian, _float3 vAxis2, _float fRadian2, _float3 vAxis3, _float fRadian3);
	void Turn(_float3 vAxis, _float fTimeDelta);

	void RotationSpot(_float3 vAxisPos, _float3 vDisVec, _float fAngle);

	void LookAt(_float3 vTargetPos);
	void LookAtForLandObject(_float3 vTargetPos);
	void Chase(_float3 vTargetPos, _float fTimeDelta);

	void CulRUByLook(_float3 vLook);
	void Set_TransformDesc(TRANSFORMDESC Desc) { m_TransformDesc = Desc; }


private:
	_float4x4			m_WorldMatrix;
	TRANSFORMDESC		m_TransformDesc;

	_float				m_fVel_Y = 0.f;
	_float				m_fGravity = 1.f;
	_float				m_fTimeAcc = 0.f;
	
	_float3				m_vOriginScale;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
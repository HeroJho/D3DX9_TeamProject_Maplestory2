#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"

#include "CutSceneManager.h"

CCamera_Free::CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
	, m_pTarget(nullptr)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs, CTransform::TRANSFORMDESC * pArg)
	: CCamera(rhs, pArg)
	, m_pTarget(nullptr)
	, m_eCamMode(CAM_PLAYER)
	, m_fDelayTimeAcc(0.f)
	, m_fFocusTime(0.f)
	, m_fShakeTimeAcc(0.f)
	, m_fShakeTime(0.f)
	, m_bShake(false)
	, m_fShortShakeTimeAcc(0.f)
	, m_fShortShakeTime(0.f)
	, m_fShakePower(0.f)
	, m_bShortShake(false)
{
	ZeroMemory(&m_vPlayerChasePos, sizeof(_float3));
	ZeroMemory(&m_vFocusPos, sizeof(_float3));
	ZeroMemory(&m_vShakeDir, sizeof(_float3));
}






HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	

	m_pTransformCom->Rotation(_float3{ 1.f,0.f,0.f }, 210.f);
	// Set_MaxPlayerCam();
	Set_MinPlayerCam();

	return S_OK;
}








void CCamera_Free::Tick(_float fTimeDelta)
{

	switch (m_eCamMode)
	{
	case CAM_FREE :
		FreeMode(fTimeDelta);
		break;
	case CAM_FOCUS:
		FocusMode(fTimeDelta);
		break;
	case CAM_PLAYER:
		ChaseModeByPlayer(fTimeDelta);
		break;
	case CAM_CUTSCENE:
		break;
	case CAM_END:
		break;
	}


	Tick_Shake(fTimeDelta);




	__super::Tick(fTimeDelta);

}

void CCamera_Free::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}








void CCamera_Free::Start_AttackShaking()
{
	StartShake(0.3f, 0.05f, 2.f, _float3(0.f, 0.f, 1.f));
}
void CCamera_Free::Start_AttackShaking2()
{
	StartShake(0.5f, 0.1f, 4.f, _float3(0.f, 0.f, 1.f));
}

void CCamera_Free::StartShake(_float fShakeTime, _float fShortShakeTime, _float fShakePower, _float3 vShakeDir)
{
	m_fShakeTimeAcc = 0.f;
	m_fShakeTime = fShakeTime;

	m_fShortShakeTimeAcc = 0.f;
	m_fShortShakeTime = fShortShakeTime;
	m_fShakePower = fShakePower;

	D3DXVec3Normalize(&vShakeDir, &vShakeDir);
	m_vShakeDir = vShakeDir;

	m_bShake = true;
}

void CCamera_Free::SetSpeed(_float fSpeed)
{
	CTransform::TRANSFORMDESC DashDesc;
	DashDesc.fSpeedPerSec = fSpeed;
	DashDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_pTransformCom->Set_TransformDesc(DashDesc);
}

void CCamera_Free::FreeMode(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_DIKState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (pGameInstance->Get_DIKState(DIK_A) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Get_DIKState(DIK_D) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	_long	MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), MouseMove * fTimeDelta * 0.05f);		
	}

	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * 0.05f);
	}

	Safe_Release(pGameInstance);
}

void CCamera_Free::FocusMode(_float fTimeDelta)
{
	m_fDelayTimeAcc += 1.f * fTimeDelta;
	if (m_fDelayTimeAcc > m_fFocusTime)
	{
		m_eCamMode = CAM_PLAYER;
		m_fDelayTimeAcc = 0.f;
		return;
	}



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);



	if (nullptr == m_pTarget)
		return;

	CTransform* pTransform = (CTransform*)m_pTarget->Get_ComponentPtr(TEXT("Com_Transform"));
	_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vChaseCamPos = vPos + m_vFocusPos;
	_float3 vCamPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	// 1. 카메라와 체이스포스 벡터를 구한다.
	_float3 vDisVec = vChaseCamPos - vCamPos;
	_float  fDis = D3DXVec3Length(&vDisVec);


	// 2. 거리가 일정 값보다 크다면
	if (0.1f < fDis)
	{
		// 3. 일정 속도로 해당 벡터로 이동한다.
		m_pTransformCom->Go_Dir(vDisVec, fDis * 3.0f, fTimeDelta);
		m_pTransformCom->LookAt(vPos);
	}



	Safe_Release(pGameInstance);
} 

void CCamera_Free::ChaseModeByPlayer(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);




	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);
	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vChaseCamPos = vPlayerPos + m_vPlayerChasePos;
	_float3 vCamPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	// 1. 카메라와 체이스포스 벡터를 구한다.
	_float3 vDisVec = vChaseCamPos - vCamPos;
	_float  fDis = D3DXVec3Length(&vDisVec);


	// 2. 거리가 일정 값보다 크다면
	if (0.1f < fDis)
	{
		// 3. 일정 속도로 해당 벡터로 이동한다.
		m_pTransformCom->Go_Dir(vDisVec, fDis * 3.0f, fTimeDelta);
		
		// 이거 신기함
		// m_pTransformCom->LookAt(vPlayerPos);

	}

    

	// 내적을 사용해서 카메라 Look을 맞춘다.
	_float3 vCamLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 vChaseCamLook = vPlayerPos - vChaseCamPos;
	D3DXVec3Normalize(&vChaseCamLook, &vChaseCamLook);
	D3DXVec3Normalize(&vCamLook, &vCamLook);

	_float fDot = D3DXVec3Dot(&vCamLook, &vChaseCamLook);
	if (0.999f > fDot)
	{
		m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), fTimeDelta);
	}


	Safe_Release(pGameInstance);
}

void CCamera_Free::Tick_Shake(_float fTimeDelta)
{
	if (!m_bShake)
		return;

	m_fShakeTimeAcc += fTimeDelta;
	if (m_fShakeTimeAcc > m_fShakeTime)
	{
		m_bShake = false;
		m_bShortShake = false;
		m_fShakeTimeAcc = 0.f;
		m_fShortShakeTimeAcc = 0.f;
	}


	m_fShortShakeTimeAcc += fTimeDelta;
	if (m_fShortShakeTimeAcc > m_fShortShakeTime)
	{
		if (m_bShortShake)
			m_bShortShake = false;
		else
			m_bShortShake = true;

		m_fShortShakeTimeAcc = 0.f;
	}


	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (m_bShortShake)
	{
		vPos += m_fShakePower * m_vShakeDir * fTimeDelta;
	}
	else
	{
		vPos -= m_fShakePower * m_vShakeDir * fTimeDelta;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

}







HRESULT CCamera_Free::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.5f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	m_pTransformCom->Set_TransformDesc(TransformDesc);

	return S_OK;
}

CCamera_Free * CCamera_Free::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free*		pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void* pArg)
{
	CCamera::CAMERADESC*		pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Free*		pInstance = new CCamera_Free(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();


}

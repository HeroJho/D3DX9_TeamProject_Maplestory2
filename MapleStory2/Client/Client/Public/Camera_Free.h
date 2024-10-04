#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
public:
	enum CAMMODE { CAM_FREE, CAM_PLAYER, CAM_FOCUS, CAM_CUTSCENE, CAM_END };

private:
	CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_Free(const CCamera_Free& rhs, CTransform::TRANSFORMDESC* pArg);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CTransform* Get_Transform() { return m_pTransformCom; }

	void SetFocus(CGameObject* pTarget, _float3 vFocusPos, _float fTime) {
		m_vFocusPos = vFocusPos;
		m_fFocusTime = fTime;
		SetTarget(pTarget);
		SetCamMode(CAM_FOCUS);
	}

	void Start_AttackShaking();
	void Start_AttackShaking2();
	
	void Set_MinPlayerCam() { m_vPlayerChasePos = _float3{ 0.f, 7.f, -8.f }; }
	void Set_MaxPlayerCam() { m_vPlayerChasePos = _float3{ 0.f, 9.f, -10.f }; }
	void Set_EndPlayerCam() { m_vPlayerChasePos = _float3{ 0.f, 11.f, -12.f }; }


public:
	void SetTarget(CGameObject* pTarget) { m_pTarget = pTarget; }
	void SetCamMode(CAMMODE eCamMode) { m_eCamMode = eCamMode; }

	// For.Shake
	void StartShake(_float fShakeTime, _float fShortShakeTime, _float fShakePower, _float3 vShakeDir);

	void SetSpeed(_float fSpeed);

public:
	void FreeMode(_float fTimeDelta);
	void FocusMode(_float fTimeDelta);
	void ChaseModeByPlayer(_float fTimeDelta);

	void Tick_Shake(_float fTimeDelta);


private:
	CAMMODE m_eCamMode;
	CGameObject*	m_pTarget;

	// For.ChaseModeByPlayer()
	_float3 m_vPlayerChasePos;

	// For.FocuseMode()
	_float m_fDelayTimeAcc;
	_float3 m_vFocusPos;
	_float m_fFocusTime;

	// For.Shake()
	_float m_fShakeTimeAcc;
	_float m_fShakeTime;
	_bool	m_bShake;

	_float m_fShortShakeTimeAcc;
	_float m_fShortShakeTime;
	_float m_fShakePower;
	_float3 m_vShakeDir;
	_bool m_bShortShake;

private:
	HRESULT SetUp_Components();

public:
	static CCamera_Free* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
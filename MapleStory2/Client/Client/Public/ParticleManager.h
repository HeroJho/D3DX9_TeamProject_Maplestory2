#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "DamageGen.h"

BEGIN(Client)

class CParticleManager final : CBase
{
	DECLARE_SINGLETON(CParticleManager)

private:
	CParticleManager();
	virtual ~CParticleManager() = default;


public:
	void Player_Walk(_float3 _vPos, _float3 _vDirVec);
	void Player_Lend(_float3 _vPos);

	void DieModel_Lend(_float3 _vPos);

	void BlueMushRoom_Lend(_float3 _vPos);

	void Shot(_float3 _vPos, _float3 _vDirVec);
	void BackShot(_float3 _vPos, _float3 _vDirVec);
	void MakeDie(_float3 vPosision);

	void UpAir(_float3 _vPos);

public:
	void MakeParticle(_float3 _vPos, _float _fScale, _float3 _vDirVec, _float _fForce_Y, _float _fForce_X, _bool _bGravity, _float _fGravityVal, _float _fLifeTime, _tchar* pTag);

	void MakeDamageGen(_int sMinDamage, _int sMaxDamage, _int iCount, _float fDelayTime, _float3 vPos, _float fLifeTime, _bool bIm, CDamage::DAMAGETYPE eType);

	void MakeLeveUp(_float3 vPos);

public:
	virtual void Free() override;
};

END
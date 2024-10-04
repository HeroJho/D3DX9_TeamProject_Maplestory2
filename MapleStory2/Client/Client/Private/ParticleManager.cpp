#include "stdafx.h"
#include "..\Public\ParticleManager.h"

#include "GameInstance.h"
#include "ToolManager.h"
#include "Particle.h"


IMPLEMENT_SINGLETON(CParticleManager)

CParticleManager::CParticleManager()
{
	
}


void CParticleManager::Player_Walk(_float3 _vPos, _float3 _vDirVec)
{
	_float3 vStartPos = _vPos;
	vStartPos.y -= 0.5f;

	MakeParticle(vStartPos, 0.15f, _vDirVec, 2.0f, 0.8f, true, 0.6f, 0.5f, nullptr);
}

void CParticleManager::Player_Lend(_float3 _vPos)
{
	_float3 vStartPos = _vPos;
	vStartPos.y -= 0.5f;

	_float fDegree = 0.f;
	for (_int i = 0; i < 4; i++)
	{
		fDegree = (_float)i * 90.f;
		
		_float3 vDirVec = { 1.f, 0.f, 0.f };

		_float4x4 Mat_Y;
		D3DXMatrixIdentity(&Mat_Y);

		D3DXMatrixRotationY(&Mat_Y, fDegree);
		D3DXVec3TransformNormal(&vDirVec, &vDirVec, &Mat_Y);

		MakeParticle(vStartPos, 0.15f, vDirVec, 2.0f, 1.0f, true, 0.6f, 0.7f, nullptr);
	}

}

void CParticleManager::DieModel_Lend(_float3 _vPos)
{
	_float3 vStartPos = _vPos;
	vStartPos.y -= 0.5f;

	_float fDegree = 0.f;
	for (_int i = 0; i < 6; i++)
	{
		fDegree = (_float)i * 60.f;

		_float3 vDirVec = { 1.f, 0.f, 0.f };

		_float4x4 Mat_Y;
		D3DXMatrixIdentity(&Mat_Y);

		D3DXMatrixRotationY(&Mat_Y, fDegree);
		D3DXVec3TransformNormal(&vDirVec, &vDirVec, &Mat_Y);

		MakeParticle(vStartPos, 0.6f, vDirVec, 2.0f, 1.0f, true, 0.1f, 0.7f, TEXT("Prototype_Component_Texture_Dust"));
	}
}

void CParticleManager::BlueMushRoom_Lend(_float3 _vPos)
{
	_float3 vStartPos = _vPos;
	vStartPos.y -= 0.5f;

	_float fDegree = 0.f;
	_float fScale = 0.f;
	_float3 vTempPos;
	for (_int i = 0; i < 10; i++)
	{
		fDegree = CGameInstance::Get_Instance()->Get_FloatRandom(0.f, 360.f);
		fScale = CGameInstance::Get_Instance()->Get_FloatRandom(0.3f, 0.6f);
		_float3 vRenPos{ CGameInstance::Get_Instance()->Get_FloatRandom(-1.f, 1.f)
		,CGameInstance::Get_Instance()->Get_FloatRandom(-1.f, 1.f)
		,CGameInstance::Get_Instance()->Get_FloatRandom(-1.f, 1.f) };
		
		_float3 vDirVec = { 1.f, 0.f, 0.f };
		vTempPos = vStartPos + vRenPos;

		_float4x4 Mat_Y;
		D3DXMatrixIdentity(&Mat_Y);

		D3DXMatrixRotationY(&Mat_Y, fDegree);
		D3DXVec3TransformNormal(&vDirVec, &vDirVec, &Mat_Y);

		MakeParticle(vTempPos, fScale, vDirVec, 3.0f, 3.5f, true, 1.f, 1.f, TEXT("Prototype_Component_Texture_Rock"));
	}
}


void CParticleManager::Shot(_float3 _vPos, _float3 _vDirVec)
{
	_float3 vStartPos = _vPos;

	_float fDegreeX = 0.f;
	_float fDegreeY = 0.f;
	for (_int i = 0; i < 4; i++)
	{
		fDegreeX = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(-5.f, 5.f);
		fDegreeY = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(-10.f, 10.f);

		_float4x4  Mat_X, Mat_Y;
		D3DXMatrixIdentity(&Mat_X);
		D3DXMatrixIdentity(&Mat_Y);


		D3DXMatrixRotationX(&Mat_X, D3DXToRadian(fDegreeX));
		D3DXVec3TransformNormal(&_vDirVec, &_vDirVec, &Mat_X);
		D3DXMatrixRotationY(&Mat_Y, D3DXToRadian(fDegreeY));
		D3DXVec3TransformNormal(&_vDirVec, &_vDirVec, &Mat_Y);

		MakeParticle(vStartPos, 0.1f, _vDirVec, 2.0f, 10.0f, true, 1.f, 0.7f, nullptr);
	}
}

void CParticleManager::BackShot(_float3 _vPos, _float3 _vDirVec)
{
	_float3 vStartPos = _vPos;

	_float fDegreeX = 0.f;
	_float fDegreeY = 0.f;
	for (_int i = 0; i < 4; i++)
	{
		fDegreeX = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(-5.f, 5.f);
		fDegreeY = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(-5.f, 5.f);

		_float4x4  Mat_X, Mat_Y;
		D3DXMatrixIdentity(&Mat_X);
		D3DXMatrixIdentity(&Mat_Y);


		D3DXMatrixRotationX(&Mat_X, D3DXToRadian(fDegreeX));
		D3DXVec3TransformNormal(&_vDirVec, &_vDirVec, &Mat_X);
		D3DXMatrixRotationY(&Mat_Y, D3DXToRadian(fDegreeY));
		D3DXVec3TransformNormal(&_vDirVec, &_vDirVec, &Mat_Y);

		MakeParticle(vStartPos, 0.1f, _vDirVec * -1.f, 2.0f, 5.0f, true, 0.2f, 0.7f, nullptr);
	}
}

void CParticleManager::UpAir(_float3 _vPos)
{
	_float3 vStartPos = _vPos;
	_float3 _vDirVec{ 0.f, 1.f, 0.f };

	_float fDegreeX = 0.f;
	_float fDegreeZ = 0.f;
	for (_int i = 0; i < 4; i++)
	{
		_float3 _vDirVec{ 0.f, 1.f, 0.f };
		fDegreeX = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(0.f, 1.f);
		fDegreeZ = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(0.f, 1.f);

		_float4x4  Mat_X, Mat_Z;
		D3DXMatrixIdentity(&Mat_X);
		D3DXMatrixIdentity(&Mat_Z);


		D3DXMatrixRotationX(&Mat_X, D3DXToRadian(fDegreeX));
		D3DXVec3TransformNormal(&_vDirVec, &_vDirVec, &Mat_X);
		D3DXMatrixRotationZ(&Mat_Z, D3DXToRadian(fDegreeZ));
		D3DXVec3TransformNormal(&_vDirVec, &_vDirVec, &Mat_Z);

		_float fScale = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(0.3f, 0.6f);
		_float vRanPos = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(-0.5f, 0.5f);
		_float vRanY = (_float)CGameInstance::Get_Instance()->Get_FloatRandom(1.f, 3.f);

		vStartPos.x += vRanPos;
		vStartPos.z += vRanPos;

		MakeParticle(vStartPos, fScale, _vDirVec, vRanY, 0.1f, true, 0.05f, 0.7f, TEXT("Prototype_Component_Texture_Dust"));
	}
}
void CParticleManager::MakeDie(_float3 vPosision)
{
	CVIBuffer_Voxel::VOXELDESC pVoxDesc;
	_float3 vPos = vPosision;
	vPos.y += 0.5f;
	pVoxDesc.vPos = vPos;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Model_Die"), LEVEL_GAMEPLAY, TEXT("Layer_Model"), &pVoxDesc)))
		return;
}


void CParticleManager::MakeParticle(_float3 _vPos, _float _fScale, _float3 _vDirVec, _float _fForce_Y, _float _fForce_X, _bool _bGravity, _float _fGravityVal, _float _fLifeTime, _tchar* pTag)
{
	CParticle::PARTICLEDESC ParticleDesc;
	ZeroMemory(&ParticleDesc, sizeof(CParticle::PARTICLEDESC));

	ParticleDesc.vPos = _vPos;
	ParticleDesc.fScale = _fScale;

	ParticleDesc.vDirVec = _vDirVec;
	ParticleDesc.fForce_X = _fForce_X;

	ParticleDesc.bGravity = _bGravity;
	ParticleDesc.fGravityVal = _fGravityVal;
	ParticleDesc.fForce_Y = _fForce_Y;

	ParticleDesc.fLifeTime = _fLifeTime;

	ParticleDesc.pTag = pTag;

	CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Particle"), LEVEL_GAMEPLAY, TEXT("Layer_Particle"), &ParticleDesc);

}

void CParticleManager::MakeDamageGen(_int sMinDamage, _int sMaxDamage, _int iCount, _float fDelayTime, _float3 vPos, _float fLifeTime, _bool bIm, CDamage::DAMAGETYPE eType)
{
	CGameInstance* pGameInstance =  CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	LEVEL eCurLevel = CToolManager::Get_Instance()->Get_CurLevel();

	CDamageGen::DAMAGEGENDESC Desc;
	Desc.sMinDamage = sMinDamage;
	Desc.sMaxDamage = sMaxDamage;
	Desc.iCount = iCount;
	Desc.fDelayTime = fDelayTime;
	Desc.vPos = vPos;
	Desc.fLifeTime = fLifeTime;
	Desc.bIm = bIm;
	Desc.eType = eType;

	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_DamageGen"), eCurLevel, TEXT("Layer_Damage"), &Desc);


	Safe_Release(pGameInstance);
}

void CParticleManager::MakeLeveUp(_float3 vPos)
{
	LEVEL eCurLevel = CToolManager::Get_Instance()->Get_CurLevel();
	CCreature::CRETUREDESC Desc;
	Desc.vPos = vPos;
	CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_LeveUp"), eCurLevel, TEXT("Layer_Damage"), &Desc);
}

void CParticleManager::Free()
{

}



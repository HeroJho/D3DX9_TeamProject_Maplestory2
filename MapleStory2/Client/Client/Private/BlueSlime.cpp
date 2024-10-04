#include "stdafx.h"
#include "..\Public\BlueSlime.h"

#include "GameInstance.h"
#include "QuestManager.h"
#include "SpawnerManager.h"

CBlueSlime::CBlueSlime(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBlueSlime::CBlueSlime(const CBlueSlime & rhs)
	: CCreature(rhs)
{
}




HRESULT CBlueSlime::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBlueSlime::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_GASMonster";
	m_iHp = 10;
	m_iIndexNum = -1;

	m_iTestCount = CGameInstance::Get_Instance()->Get_Random(6, 11);

	CSpawner::SPAWNERINFO* pMonsterDesc = (CSpawner::SPAWNERINFO*)pArg;

	m_iDirection = pMonsterDesc->MonsterNum;

	m_fColRad = 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 2.f, 0.f));
	m_pTransformCom->Set_Scaled(1.5f);

	SetState(STATE_CHASE, DIR_L);

	SetShadow(LEVEL_GAS, 1.2f);


	return S_OK;
}




HRESULT CBlueSlime::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.3f, 1.f, 0.3f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueSlime_Move"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueSlime_MoveR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueSlime_DieR"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlueSlime_Die"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 0.3f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CBlueSlime::Tick(_float fTimeDelta)
{


	switch (m_iDirection)
	{
	case R:
		m_pTransformCom->Chase(_float3(-6.5f, 0.f, 7.0f), fTimeDelta * 2.5f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x <= -6.4f)
		{
			Set_Dead();
		}
		break;
	case G:
		m_pTransformCom->Chase(_float3(6.5f, 0.f, 7.0f), fTimeDelta * 2.5f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >= 6.4f)
		{
			Set_Dead();
		}
		break;
	case B:
		m_pTransformCom->Chase(_float3(10.5f, 0.f, 0.f), fTimeDelta * 2.7f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x >= 10.4f)
		{
			DestroyCube(m_iTestCount);
			m_iTestCount--;
			Set_Dead();
		}
		break;
	case P:
		m_pTransformCom->Chase(_float3(-10.5f, 0.f, 0.f), fTimeDelta * 2.7f);
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x <= -10.4f)
		{
			Set_Dead();
		}
		break;
	}

	switch (m_eCurState)
	{
	case Client::CBlueSlime::STATE_IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case Client::CBlueSlime::STATE_MOVE:
		Tick_Move(fTimeDelta);
		break;
	case Client::CBlueSlime::STATE_HIT:
		Tick_Hit(fTimeDelta);
		break;
	case Client::CBlueSlime::STATE_CHASE:
		Tick_Chase(fTimeDelta);
		break;
	}

}
void CBlueSlime::LateTick(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_CHASE, m_eDir);

	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_MONSTER, this);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER, this);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	Set_Billboard();
}
HRESULT CBlueSlime::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}





void CBlueSlime::Tick_Idle(_float fTimeDelta)
{
}
void CBlueSlime::Tick_Move(_float fTimeDelta)
{
}
void CBlueSlime::Tick_Hit(_float fTimeDelta)
{
}

void CBlueSlime::Tick_Chase(_float fTimeDelta)
{
}



void CBlueSlime::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CBlueSlime::SetAni()
{
	switch (m_eCurState)
	{
	case CBlueSlime::STATE_IDLE:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
	break;
	case CBlueSlime::STATE_MOVE:
	{

	}
	break;
	case CBlueSlime::STATE_HIT:
		if(m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_DieR"), 0.5f, CAnimator::STATE_ONCE);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_Die"), 0.5f, CAnimator::STATE_ONCE);
		break;
	case CBlueSlime::STATE_CHASE:
		if (m_eDir == DIR_R)
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_MoveR"), 0.1f, CAnimator::STATE_LOOF);
		else
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlueSlime_Move"), 0.1f, CAnimator::STATE_LOOF);
		break;
	}
}

void CBlueSlime::Damaged(CGameObject * pOther)
{
}

void CBlueSlime::DestroyCube(_int iLength)
{
	_float4x4 Matrix;
	_float3 foriDir = { 1.f, 0.f, 0.f };
	_float3 fDir;
	_float3 vPos;
	for (_int i = 0; i <= 360; i += 45)
	{
		_int iAngle = i + 20;
		D3DXMatrixRotationY(&Matrix, D3DXToRadian(_float(iAngle)));
		D3DXVec3TransformNormal(&fDir, &foriDir, &Matrix);
		vPos = fDir * iLength;
		CMap_Manager::CUBEDATA desc;
		vPos.y += 2.f;
		desc.vPos = vPos;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Trigger"), LEVEL_GAS, TEXT("Layer_Cube"), &desc)))
			return;
	}

}





CBlueSlime * CBlueSlime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlueSlime*		pInstance = new CBlueSlime(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlueSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBlueSlime::Clone(void* pArg)
{
	CBlueSlime*		pInstance = new CBlueSlime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlueSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBlueSlime::Collision(CGameObject * pOther)
{

}




void CBlueSlime::Free()
{
	__super::Free();

}


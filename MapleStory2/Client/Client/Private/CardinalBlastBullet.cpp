#include "stdafx.h"
#include "..\Public\CardinalBlastBullet.h"
#include "CardinalBlastHit.h"
#include "GameInstance.h"

#include "ParticleManager.h"

CCardinalBlastBullet::CCardinalBlastBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CCardinalBlastBullet::CCardinalBlastBullet(const CCardinalBlastBullet & rhs)
	: CCreature(rhs), m_fDeadAcc(0.f)
{
}




HRESULT CCardinalBlastBullet::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CCardinalBlastBullet::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fColRad = 0.1f;
	m_iDamage = 1;
	m_sTag = "Player_Skill";
	m_pTransformCom->Set_Scaled(_float3{ 3.f,1.5f,1.5f });
	
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_CardinalBlast_Bullet"), 1.f, CAnimator::STATE_LOOF);

	memcpy(&m_Desc, pArg, sizeof(CARDINALBULLETDESC));
	m_eDir = m_Desc.eDir;

	SetPosition(m_eDir);
	m_fYDistance = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	return S_OK;
}




HRESULT CCardinalBlastBullet::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 1.f, 1.f, 1.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CardinalBlast_Bullet"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CCardinalBlastBullet::Tick(_float fTimeDelta)
{

	MoveAttack(fTimeDelta);

}
void CCardinalBlastBullet::LateTick(_float fTimeDelta)
{
	m_fDeadAcc += 1.f * fTimeDelta;
	if (m_fDeadAcc > 5.f)
		Set_Dead();
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);
	__super::BoxColCom_Tick(m_pTransformCom);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_PLAYER_SKILL, this);
	SetDirection();
}
HRESULT CCardinalBlastBullet::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	//__super::BoxColCom_Render(m_pTransformCom);

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}


void CCardinalBlastBullet::SetState(STATE eState, DIR eDir)
{

}
void CCardinalBlastBullet::SetDirection()
{
	switch (m_eDir)
	{
	case Client::CCreature::DIR_L:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 0.f, _float3(1.f, 0.f, 0.f), 90.f);
		break;
	case Client::CCreature::DIR_R:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 180.f, _float3(-1.f, 0.f, 0.f), 90.f);
		break;
	case Client::CCreature::DIR_U:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 90.f, _float3(0.f, 0.f, -1.f), 90.f);
		break;
	case Client::CCreature::DIR_D:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 270.f, _float3(0.f, 0.f, 1.f), 90.f);
		break;
	case Client::CCreature::DIR_LU:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 45.f, _float3(1.f, 0.f, -1.f), 90.f);
		break;
	case Client::CCreature::DIR_RU:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 135.f, _float3(-1.f, 0.f, -1.f), 90.f);
		break;
	case Client::CCreature::DIR_LD:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), -45.f, _float3(1.f, 0.f, 1.f), 90.f);
		break;
	case Client::CCreature::DIR_RD:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 225.f, _float3(-1.f, 0.f, 1.f), 90.f);
		break;
	case Client::CCreature::DIR_END:
		break;
	default:
		break;
	}
}
void CCardinalBlastBullet::SetPosition(DIR eDir)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);
	_float RD = pInstance->Get_FloatRandom(-0.5f, 0.5f);
	_float RD2 = pInstance->Get_FloatRandom(-0.5f, 0.5f);
	CTransform* m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	Safe_Release(pInstance);

	Safe_Release(pInstance);
	_float3 vPosFix;
	switch (eDir)
	{
	case Client::CCreature::DIR_L:
		vPosFix = { -1.f,-0.01f,RD };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_R:
		vPosFix = { 1.f,-0.01f,RD };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_U:
		vPosFix = { RD,-0.01f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_D:
		vPosFix = { RD,-0.01f,-1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LU:
		vPosFix = { -1.f+RD2,-0.01f,1.f+RD2 };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RU:
		vPosFix = { 1.f+RD,-0.01f,1.f+RD2 };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LD:
		vPosFix = { -1.f+RD,-0.01f,-1.f+RD2 };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RD:
		vPosFix = { 1.f+RD2,-0.01f,-1.f+RD2 };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_END:

		break;
	default:
		break;
	}
}
void CCardinalBlastBullet::MoveAttack(_float fTimeDelta)
{
	switch (m_eDir)
	{
	case Client::CCreature::DIR_L:
		m_pTransformCom->Go_L(fTimeDelta);
		break;
	case Client::CCreature::DIR_R:
		m_pTransformCom->Go_R(fTimeDelta);
		break;
	case Client::CCreature::DIR_U:
		m_pTransformCom->Go_U(fTimeDelta);
		break;
	case Client::CCreature::DIR_D:
		m_pTransformCom->Go_D(fTimeDelta);
		break;
	case Client::CCreature::DIR_LU:
		m_pTransformCom->Go_LU(fTimeDelta);
		break;
	case Client::CCreature::DIR_RU:
		m_pTransformCom->Go_RU(fTimeDelta);
		break;
	case Client::CCreature::DIR_LD:
		m_pTransformCom->Go_LD(fTimeDelta);
		break;
	case Client::CCreature::DIR_RD:
		m_pTransformCom->Go_RD(fTimeDelta);
		break;
	case Client::CCreature::DIR_END:
		break;
	default:
		break;
	}
}
void CCardinalBlastBullet::SetAni()
{

}






CCardinalBlastBullet * CCardinalBlastBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCardinalBlastBullet*		pInstance = new CCardinalBlastBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCardinalBlastBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CCardinalBlastBullet::Clone(void* pArg)
{
	CCardinalBlastBullet*		pInstance = new CCardinalBlastBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCardinalBlastBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CCardinalBlastBullet::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Monster")
	{

		if (1 < m_pOther.size())
			return;

		for (auto& TempOther : m_pOther)
		{
			if (TempOther == pOther)
				return;
		}

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTransform = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_CardinalBlast_Hit"), LEVEL_STATIC, TEXT("Layer_Player_Skill"), &pTransform->Get_State(CTransform::STATE_POSITION));
		Safe_Release(pGameInstance);


		m_pOther.push_back(pOther);
	
		_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		vPos.y += 1.5f;

		CParticleManager::Get_Instance()->MakeDamageGen(500000, 1500000, 4, 0.05f, vPos, 1.6f, true, CDamage::DAMAGE_ATTACKED);
		pOther->Damaged(this);

		Set_Dead();
	}
}

HRESULT CCardinalBlastBullet::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CCardinalBlastBullet::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CCardinalBlastBullet::Free()
{
	__super::Free();
}


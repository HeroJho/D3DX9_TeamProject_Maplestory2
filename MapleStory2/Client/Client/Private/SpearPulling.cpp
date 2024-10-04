#include "stdafx.h"
#include "..\Public\SpearPulling.h"
#include "SpearPullingHit.h"
#include "GameInstance.h"
#include "ParticleManager.h"

CSpearPulling::CSpearPulling(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CSpearPulling::CSpearPulling(const CSpearPulling & rhs)
	: CCreature(rhs)
{
}




HRESULT CSpearPulling::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CSpearPulling::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);
	m_sTag = "Player_Skill";
	m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	Safe_Release(pInstance);

	m_fColRad = 0.5f;
	
	m_pTransformCom->Set_Scaled(5.f);
	m_pTransformCom->Set_ScaledX(3.5f);

	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_SpearPulling"), 0.07f, CAnimator::STATE_LOOF);
	memcpy(&m_Desc, pArg, sizeof(SPEARPULLINGDESC));
	m_eDir = m_Desc.eDir;
	SetDirection();
	SetPosition(m_eDir);
	m_bPulling = false;
	m_fPullAcc = 0.f;
	m_fYDistance = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	m_iDamage = 2;
	return S_OK;
}




HRESULT CSpearPulling::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 3.5f, 3.f, 3.5f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;


	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SpearPulling"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 4.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CSpearPulling::Tick(_float fTimeDelta)
{
	SetPosition(m_eDir);
	m_fPullAcc += 1.f * fTimeDelta;
	if (m_fPullAcc > 0.5f)
	{
		m_bPulling = true;
		m_fPullAcc = 0.f;
	}
}
void CSpearPulling::LateTick(_float fTimeDelta)
{

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);

	if (m_pAnimatorCom->Get_AnimCount() == 11)
	{
		Set_Dead();
	}
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	__super::BoxColCom_Tick(m_pTransformCom);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_PLAYER_SKILL, this);
	//m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_PLAYER_SKILL, this);

}
HRESULT CSpearPulling::Render()
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



	//__super::BoxColCom_Render(m_pTransformCom);




	return S_OK;
}



void CSpearPulling::SetState(STATE eState, DIR eDir)
{

}
void CSpearPulling::SetDirection()
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
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 90.f, _float3(0.f, 0.f, 1.f), 90.f);
		break;
	case Client::CCreature::DIR_D:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 270.f, _float3(0.f, 0.f, -1.f), 90.f);
		break;
	case Client::CCreature::DIR_LU:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 45.f, _float3(-1.f, 0.f, 1.f), 90.f);
		break;
	case Client::CCreature::DIR_RU:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 135.f, _float3(1.f, 0.f, 1.f), 90.f);
		break;
	case Client::CCreature::DIR_LD:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), -45.f, _float3(-1.f, 0.f, -1.f), 90.f);
		break;
	case Client::CCreature::DIR_RD:
		m_pTransformCom->RotationTwo(_float3(0.f, 1.f, 0.f), 225.f, _float3(1.f, 0.f, -1.f), 90.f);
		break;
	case Client::CCreature::DIR_END:
		break;
	default:
		break;
	}


}
void CSpearPulling::SetPosition(DIR eDir)
{
	_float3 vPosFix;
	switch (eDir)
	{
	case Client::CCreature::DIR_L:
		vPosFix = { -2.f,0.f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_R:
		vPosFix = { 2.f,0.f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_U:
		vPosFix = { 0.f,0.f,2.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_D:
		vPosFix = { 0.f,0.f,-2.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LU:
		vPosFix = { -1.f,0.f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RU:
		vPosFix = { 1.f,0.f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LD:
		vPosFix = { -1.f,0.f,-1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RD:
		vPosFix = { 1.f,0.f,-1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_END:

		break;
	default:
		break;
	}
}
void CSpearPulling::SetAni()
{

}


CSpearPulling * CSpearPulling::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpearPulling*		pInstance = new CSpearPulling(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSpearPulling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CSpearPulling::Clone(void* pArg)
{
	CSpearPulling*		pInstance = new CSpearPulling(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSpearPulling"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CSpearPulling::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Monster")
	{
		if (m_bPulling)
		{
			if (5 < m_pOther.size())
				return;

			for (auto& TempOther : m_pOther)
			{
				if (TempOther == pOther)
					return;
			}

			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CSpearPullingHit::SPEARPULLINGHITDESC SpearPullingHitDesc;
			CTransform* pTransform = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));
			SpearPullingHitDesc.vPos = pTransform->Get_State(CTransform::STATE_POSITION);

			pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SpearPulling_Hit"), LEVEL_STATIC, TEXT("Layer_Player_Skill"), &SpearPullingHitDesc);
			Safe_Release(pGameInstance);

			m_pOther.push_back(pOther);

			_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
			vPos.y += 1.5f;

			CParticleManager::Get_Instance()->MakeDamageGen(100000, 9000000, 1, 0.05f, vPos, 1.6f, true, CDamage::DAMAGE_ATTACKED);
			pOther->Damaged(this);
			Pulling(pOther);
		}

		//Set_Dead();
	}
}

void CSpearPulling::Pulling(CGameObject * pOther)
{
	CTransform* pTrans = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));
	_float3 vLookTemp = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pTrans->Get_State(CTransform::STATE_POSITION);
	pTrans->Set_State(CTransform::STATE_POSITION, pTrans->Get_State(CTransform::STATE_POSITION) + vLookTemp);
}

HRESULT CSpearPulling::Set_RenderState()
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

HRESULT CSpearPulling::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CSpearPulling::Free()
{
	__super::Free();
}


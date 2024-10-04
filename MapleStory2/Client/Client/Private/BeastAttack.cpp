#include "stdafx.h"
#include "..\Public\BeastAttack.h"
#include "BeastHit.h"
#include "GameInstance.h"
#include "ParticleManager.h"

CBeastAttack::CBeastAttack(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CBeastAttack::CBeastAttack(const CBeastAttack & rhs)
	: CCreature(rhs), m_bCreate(false)
{
}




HRESULT CBeastAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CBeastAttack::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_sTag = "Player_Skill";
	m_iDamage = 10;

	m_pTransformCom->Set_Scaled(_float3{ 10.f,8.f,10.f });

	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Beast_Attack"), 0.1f, CAnimator::STATE_ONCE);
	memcpy(&m_Desc, pArg, sizeof(BEASTATTACKDESC));
	m_eDir = m_Desc.eDir;
	m_bSound = false;
	m_pTarget = m_Desc.pTransform;
	m_fYDistance = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION));
	return S_OK;
}




HRESULT CBeastAttack::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 6.f, 5.f, 3.f };
	BoxColDesc.vPivot = _float3{ 0.f, -1.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Beast_Attack"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}




void CBeastAttack::Tick(_float fTimeDelta)
{
	
}
void CBeastAttack::LateTick(_float fTimeDelta)
{

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);
	__super::BoxColCom_Tick(m_pTransformCom);
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_PLAYER_SKILL, this);

	if (m_pAnimatorCom->Get_AnimCount() == 12)
	{
		if (!m_bSound)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"BeastEnd.mp3", 21, 1.f);
			m_bSound = true;
		}
	}

	if (m_pAnimatorCom->Get_AnimCount() == 22)
	{
		Set_Dead();
	}


}
HRESULT CBeastAttack::Render()
{
	Set_Billboard();

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


void CBeastAttack::SetState(STATE eState, DIR eDir)
{

}

void CBeastAttack::SetPosition(DIR eDir)
{
	_float3 vPosFix;
	switch (eDir)
	{
	case Client::CCreature::DIR_L:
		vPosFix = { -1.f,2.f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_R:
		vPosFix = { 1.f,2.f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_U:
		vPosFix = { 0.f,0.f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_D:
		vPosFix = { 0.f,0.f,-1.f };
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
void CBeastAttack::SetAni()
{

}



CBeastAttack * CBeastAttack::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBeastAttack*		pInstance = new CBeastAttack(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBeastAttack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBeastAttack::Clone(void* pArg)
{
	CBeastAttack*		pInstance = new CBeastAttack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBeastAttack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CBeastAttack::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Monster")
	{

		if (10 < m_pOther.size())
			return;

		for (auto& TempOther : m_pOther)
		{
			if (TempOther == pOther)
				return;
		}
		if (m_pAnimatorCom->Get_AnimCount() == 12)
		{
			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			
			CTransform* pTransform = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));
			pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Beast_Hit"), LEVEL_STATIC, TEXT("Layer_Player_Skill"), &pTransform->Get_State(CTransform::STATE_POSITION));
			Safe_Release(pGameInstance);
			m_pOther.push_back(pOther);

			_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
			vPos.y += 1.5f;

			CParticleManager::Get_Instance()->MakeDamageGen(1000000, 9000000, 10, 0.05f, vPos, 1.6f, false, CDamage::DAMAGE_ATTACKED);
			pOther->Damaged(this);
		}

		//Set_Dead();
	}
}

HRESULT CBeastAttack::Set_RenderState()
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

HRESULT CBeastAttack::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CBeastAttack::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "..\Public\ChasingShotBullet.h"
#include "ChasingShotHit.h"
#include "GameInstance.h"

#include "ParticleManager.h"

CChasingShotBullet::CChasingShotBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CChasingShotBullet::CChasingShotBullet(const CChasingShotBullet & rhs)
	: CCreature(rhs), m_fDeadAcc(0.f)
{
}




HRESULT CChasingShotBullet::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CChasingShotBullet::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Set_Scaled(_float3{ 6.f,3.5f,6.5f });
	m_sTag = "Player_Skill";
	m_iDamage = 4;
	m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_ChasingShot_Bullet"), 1.f, CAnimator::STATE_LOOF);

	memcpy(&m_Desc, pArg, sizeof(CHASINGBULLETDESC));
	m_eDir = m_Desc.eDir;

	SetPosition(m_eDir);
	m_fYDistance = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	return S_OK;
}




HRESULT CChasingShotBullet::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 3.f, 2.f, 3.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChasingShot_Bullet"), nullptr);
	}


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}




void CChasingShotBullet::Tick(_float fTimeDelta)
{

	MoveAttack(fTimeDelta);

}
void CChasingShotBullet::LateTick(_float fTimeDelta)
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
HRESULT CChasingShotBullet::Render()
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


void CChasingShotBullet::SetState(STATE eState, DIR eDir)
{

}
void CChasingShotBullet::SetDirection()
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
void CChasingShotBullet::SetPosition(DIR eDir)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	CTransform* m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	Safe_Release(pInstance);

	Safe_Release(pInstance);
	_float3 vPosFix;
	switch (eDir)
	{
	case Client::CCreature::DIR_L:
		vPosFix = { -1.f,-0.01f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_R:
		vPosFix = { 1.f,-0.01f,0.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_U:
		vPosFix = { 0.f,-0.01f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_D:
		vPosFix = { 0.f,-0.01f,-1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LU:
		vPosFix = { -1.f,-0.01f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RU:
		vPosFix = { 1.f,-0.01f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_LD:
		vPosFix = { -1.f,-0.01f,-1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_RD:
		vPosFix = { 1.f,-0.01f,-1.f };
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget->Get_State(CTransform::STATE_POSITION) + vPosFix);
		break;
	case Client::CCreature::DIR_END:

		break;
	default:
		break;
	}
}
void CChasingShotBullet::MoveAttack(_float fTimeDelta)
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
void CChasingShotBullet::SetAni()
{

}






CChasingShotBullet * CChasingShotBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChasingShotBullet*		pInstance = new CChasingShotBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CChasingShotBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CChasingShotBullet::Clone(void* pArg)
{
	CChasingShotBullet*		pInstance = new CChasingShotBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CChasingShotBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CChasingShotBullet::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Monster")
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

		CTransform* pTransform = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ChasingShot_Hit"), LEVEL_STATIC, TEXT("Layer_Player_Skill"), &pTransform->Get_State(CTransform::STATE_POSITION));
		Safe_Release(pGameInstance);


		m_pOther.push_back(pOther);
	
		_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		vPos.y += 1.5f;

		CParticleManager::Get_Instance()->MakeDamageGen(15000000, 95000000, 8, 0.05f, vPos, 1.6f, false, CDamage::DAMAGE_ATTACKED);
		pOther->Damaged(this);

		//Set_Dead();
	}
}

HRESULT CChasingShotBullet::Set_RenderState()
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

HRESULT CChasingShotBullet::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}




void CChasingShotBullet::Free()
{
	__super::Free();
}


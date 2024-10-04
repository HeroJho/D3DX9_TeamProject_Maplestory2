#include "stdafx.h"
#include "..\Public\BlackWizard.h"
#include "GameInstance.h"
#include "BlackWizardAttack1.h"
#include "UIManager.h"
#include "ToolManager.h"
#include "QuestManager.h"


CBlackWizard::CBlackWizard(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCreature(pGraphic_Device)
{
}

CBlackWizard::CBlackWizard(const CBlackWizard & rhs)
	:CCreature(rhs)
{
}

HRESULT CBlackWizard::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CBlackWizard::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	m_pTarget = (CTransform*)pInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	Safe_Release(pInstance);

	m_sTag = "Tag_Monster";
	m_iDamage = 1;
	m_eCurState = STATE_APPEAR;
	m_iHp = 100;
	CUIManager::Get_Instance()->Set_BlackWizardHp(m_iHp);
	SetAni();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 3.f, 0.5f));
	m_pTransformCom->Set_Scaled(13.f);
	m_bState = false;
	m_bFinal = false;
	m_bSound = false;
	m_bSceneEnd = false;
	m_fSceneAcc = 0.f;
	SetShadow(LEVEL_DARKMAGE, 1.5f);
	return S_OK;
}

void CBlackWizard::Tick(_float fTimeDelta)
{
	if (!m_bSceneEnd)
	{
		m_fSceneAcc += 1.f * fTimeDelta;
		if (m_fSceneAcc > 15)
		{
			m_bSceneEnd = true;
			m_fSceneAcc = 0.f;
		}
	}
	MakeBlockCube();

	if (m_bDead)
	{
		m_fDead += 1.f * fTimeDelta;
		if (m_fDead > 8)
		{
			CToolManager::Get_Instance()->SetDestLevel(LEVEL_WHITEMAGE, _float3{ 2.f, 2.f, -2.f });
		}
	}
}

void CBlackWizard::LateTick(_float fTimeDelta)
{

	
	if (m_bFinal)
	{
		m_eCurState = STATE_STAND;
		SetAni();
	}

	else if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
	{
		Pattern();
		if (m_eCurState != STATE_END)
			SetState();
	}
	
	


	Fix_Scale();

	

	__super::BoxColCom_Tick(m_pTransformCom);
	
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_MONSTER, this);

	if (m_eCurState != STATE_END)
	{
		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BOSS, this);
	}

	m_iHp = CUIManager::Get_Instance()->Get_BlackWizardHp();

	if (m_iHp <= 0)
	{
		CQuestManager::Get_Instance()->Hunting(TEXT("BlackMage"));
		m_eCurState = STATE_ESCAPE;
		SetAni();
	}

}

HRESULT CBlackWizard::Render()
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

void CBlackWizard::Collision(CGameObject * pOther)
{
}

void CBlackWizard::SetState()
{
	STATE State = (STATE)CGameInstance::Get_Instance()->Get_Random(STATE_STAND, STATE_END - 1);
	if (m_eCurState == State)
		m_bState = false;
	else
		m_bState = true;
	m_eCurState = State;
	SetAni();
}

void CBlackWizard::Pattern()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	switch (m_eCurState)
	{
	case STATE_APPEAR:
		break;
	case STATE_ESCAPE:
		m_eCurState = STATE_END;
		m_bDead = true;
		break;
	case STATE_STAND:
		break;
	case STATE_SKILL1:
		if(m_bState)
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack1"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"));
		break;
	case STATE_SKILL2:
		if (m_bState)
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack2"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"));
		break;
	case STATE_SKILL3:
		if (m_bState)
		Skill3();
		break;
	case STATE_SKILL4:
		if (m_bState)
		Skill4();
		break;
	case STATE_END:
		break;
	default:
		break;
	}
	Safe_Release(pGameInstance);
}

void CBlackWizard::Fix_Scale()
{
	switch (m_eCurState)
	{
	case Client::CBlackWizard::STATE_APPEAR:
		m_pTransformCom->Set_Scaled(13.f);
		break;
	case Client::CBlackWizard::STATE_ESCAPE:
		m_pTransformCom->Set_Scaled(13.f);
		break;
	/*case Client::CBlackWizard::STATE_STAND:
		m_pTransformCom->Set_Scaled(_float3{ 7.5f,9.f,7.5f });
		break;
	case Client::CBlackWizard::STATE_SKILL1:
		m_pTransformCom->Set_Scaled(_float3{ 9.f,10.f,9.f });
		break;
	case Client::CBlackWizard::STATE_SKILL2:
		m_pTransformCom->Set_Scaled(_float3{ 10.f,9.f,9.f });
		break;
	case Client::CBlackWizard::STATE_SKILL3:
		m_pTransformCom->Set_Scaled(_float3{ 11.f,9.f,9.f });
		break;
	case Client::CBlackWizard::STATE_SKILL4:
		m_pTransformCom->Set_Scaled(_float3{ 9.5f,9.5f,9.5f });
		break;*/
	case Client::CBlackWizard::STATE_END:
		break;
	default:
		break;
	}
}

void CBlackWizard::SetAni()
{
	switch (m_eCurState)
	{
	case Client::CBlackWizard::STATE_APPEAR:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Appear"), 0.1f, CAnimator::STATE_ONCE);
		break;
	case Client::CBlackWizard::STATE_STAND:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Stand"), 0.1f, CAnimator::STATE_ONCE);
		break;
	case Client::CBlackWizard::STATE_SKILL1:		
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Skill1"), 0.1f, CAnimator::STATE_ONCE);
		break;
	case Client::CBlackWizard::STATE_SKILL2:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Skill2"), 0.1f, CAnimator::STATE_ONCE);
		break;
	case Client::CBlackWizard::STATE_SKILL3:		
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Skill3"), 0.1f, CAnimator::STATE_ONCE);
		break;
	case Client::CBlackWizard::STATE_SKILL4:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Skill4"), 0.1f, CAnimator::STATE_ONCE);
		break;
	case Client::CBlackWizard::STATE_ESCAPE:
		if (!m_bDead)
		{
			CGameInstance::Get_Instance()->PlaySoundW(L"BlackWizardDie.mp3", 6, 1.f);
			m_bDead = true;
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_BlackWizard_Escape"), 0.2f, CAnimator::STATE_ONCE);
		}
		break;
	case Client::CBlackWizard::STATE_END:
		break;
	default:
		break;
	}
}

void CBlackWizard::Damaged(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Player_Skill")
	CUIManager::Get_Instance()->Fix_BlackWizardHp(pOther->Get_Damage()*-1);
}

void CBlackWizard::Skill3()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_float3 vPos = m_pTarget->Get_State(CTransform::STATE_POSITION);
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &vPos);
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -2.5f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 2.5f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,2.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,-2.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -5.f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 5.f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,5.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,-5.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -7.5f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 7.5f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,7.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,-7.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -10.f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 10.f,0.f,0.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,10.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 0.f,0.f,-10.f }));
	Safe_Release(pGameInstance);
}

void CBlackWizard::Skill4()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_float3 vPos = m_pTarget->Get_State(CTransform::STATE_POSITION);
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &vPos);
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -1.25f,0.f,1.25f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 1.25f,0.f,1.25f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -1.25f,0.f,-1.25f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 1.25f,0.f,-1.25f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -2.5f,0.f,2.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 2.5f,0.f,2.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -2.5f,0.f,-2.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 2.5f,0.f,-2.5f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -3.75f,0.f,3.75f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 3.75f,0.f,3.75f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -3.75f,0.f,-3.75f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 3.75f,0.f,-3.75f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -5.f,0.f,5.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 5.f,0.f,5.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ -5.f,0.f,-5.f }));
	pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_BlackWizardAttack3"), LEVEL_DARKMAGE, TEXT("Layer_Monster_Skill"), &(vPos - _float3{ 5.f,0.f,-5.f }));
	Safe_Release(pGameInstance);
}

HRESULT CBlackWizard::Set_RenderState()
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

HRESULT CBlackWizard::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBlackWizard::SetUp_Components()
{
	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 2.f, 6.f, 2.f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Appear"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Stand"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Escape"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Skill1"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Skill2"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Skill3"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BlackWizard_Skill4"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

CBlackWizard * CBlackWizard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlackWizard*	pInstance = new CBlackWizard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBlackWizard"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlackWizard::Clone(void * pArg)
{
	CBlackWizard*	pInstance = new CBlackWizard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBlackWizard"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackWizard::Free()
{
	__super::Free();
}








void CBlackWizard::MakeBlockCube()
{
	_int iNum = 3 - CToolManager::Get_Instance()->Get_BlackCubeCount();

	CMap_Manager::CUBEDATA desc;
	for (_int i = 0; i < iNum; i++)
	{
		_float fX = CGameInstance::Get_Instance()->Get_FloatRandom(-8.f, 8.f);
		_float fZ = CGameInstance::Get_Instance()->Get_FloatRandom(-8.f, 8.f);

		desc.iIndex = 1;
		desc.vPos = _float3{ fX, 3.f, fZ };

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_LaserBlockCube"), LEVEL_DARKMAGE, TEXT("Layer_Cube"), &desc)))
			return;

		CToolManager::Get_Instance()->IncreaseBlackCubeCount();
	}

}
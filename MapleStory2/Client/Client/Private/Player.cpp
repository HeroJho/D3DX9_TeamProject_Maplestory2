#include "stdafx.h"
#include "..\Public\Player.h"
#include "SolunaSlashEffectA.h"
#include "SolunaSlashEffectB.h"
#include "CrossTheStyx.h"
#include "SunCross.h"
#include "GameInstance.h"
#include "InvenManager.h"
#include "ReefAttack.h"
#include "WarriorReef.h"
#include "SpearPulling.h"
#include "ParticleManager.h"
#include "CardinalBlastAttack.h"
#include "ChasingShotAttack.h"
#include "BeastEffect.h"
#include "Item.h"
#include "Shadow.h"
#include "UIManager.h"
#include "CutSceneManager.h"
#include "QuestManager.h"
#include "Shader.h"
#include "ToolManager.h"
#include "ToolManager.h"
#include "ParticleManager.h"
#include "ItemInfo.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCreature(pGraphic_Device)
{
}
CPlayer::CPlayer(const CPlayer & rhs)
	: CCreature(rhs)
{
	ZeroMemory(&m_vRespownPos, sizeof(_float3));
	m_fRespownPosAcc = 0.f;
}


HRESULT CPlayer::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CPlayer::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_sTag = "Tag_Player";

	// ���浹 �� �ڵ�
	m_fColRad = 0.5f;

	m_fDashAcc = 0.f;
	m_fOriJumpPower = 7.f;
	m_fJumpPower = m_fOriJumpPower;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-9.f, 4.f, -3.f));
	m_pTransformCom->Set_Scaled(2.5f);

	m_vRespownPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	D3DXCreateFont(m_pGraphic_Device, 13, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L" �ڵ�", &m_Font);

	// *�߷� �ڵ�
	// �߷� ���� ����
	m_pTransformCom->Set_Gravity(1.f);
	m_pShaderCom->Set_RawValue("g_vPlayerColor", _float4{ 1.f,1.f,1.f,1.f }, sizeof(_float4));

	SetState(STATE_IDLE, DIR_D);

	// �׸��� ����
	SetShadow(LEVEL_STATIC, 2.5f);

	return S_OK;
}




HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 4.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.5f, 1.f, 0.5f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;
	

	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_U"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_D"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_L"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_R"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_LU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_RU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_LD"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Idle_RD"), nullptr);
	}
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_U"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_D"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_L"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_R"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_LU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_RU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_LD"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Move_RD"), nullptr);
	}
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_U"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_D"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_L"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_R"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_LU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_RU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_LD"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Jump_RD"), nullptr);
	}
	{
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_U"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_D"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_L"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_R"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_LU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_RU"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_LD"), nullptr);
		m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Attack_RD"), nullptr);
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Player"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}




void CPlayer::Tick(_float fTimeDelta)
{
	CUIManager::Get_Instance()->Set_StartMove(true);
	CUIManager::Get_Instance()->Set_EndMove(false);


	if (CGameInstance::Get_Instance()->Key_Down(DIK_Y))
	{
		//CMap_Manager::CUBEDATA desc;
		//desc.iIndex = 1;
		//desc.vPos = _float3{ -2.f, 1.f, -2.f };
		//if (FAILED(CGameInstance::Get_Instance()->Add_GameObjectToLayer(TEXT("Prototype_GameObject_LaserBlockCube"), LEVEL_DARKMAGE, TEXT("Layer_Cube"), &desc)))
		//	return;

		CCutSceneManager::Get_Instance()->Start_Enter_InitDHenesys();
		
	}
	else if (CGameInstance::Get_Instance()->Key_Down(DIK_T))
	{
		//CCutSceneManager::Get_Instance()->Get_MainCam()->Start_AttackShaking2();
		
	}


	switch (m_eCurState)
	{
	case Client::CPlayer::STATE_IDLE:
	{
		GetKeyInput(fTimeDelta);
		Idle(fTimeDelta);
	}
		break;
	case Client::CPlayer::STATE_MOVE:
		GetKeyInput(fTimeDelta);
		break;
	case Client::CPlayer::STATE_JUMP:
		GetJumpKeyInput(fTimeDelta);
		Jump(fTimeDelta);
		break;
	case Client::CPlayer::STATE_ATTACK:		
		break;
	case Client::CPlayer::STATE_DASH:
		Dash(fTimeDelta);
		break;
	case Client::CPlayer::STATE_DJUMP:
		DoubleJump(fTimeDelta);
		break;
	case Client::CPlayer::STATE_KNOCKBACK:
		TickKnockBack(fTimeDelta);
		break;
	case Client::CPlayer::STATE_DIE:
		Die(fTimeDelta);
		break;
	}

	Shading(fTimeDelta);

	GetKeyCheate(fTimeDelta);
	CheckAttakedTime(fTimeDelta);
	Particle(fTimeDelta);

}
void CPlayer::LateTick(_float fTimeDelta)
{

	LEVEL eCurLevel = CToolManager::Get_Instance()->Get_CurLevel();
	if (eCurLevel == LEVEL_DHENESYSHUNTING)
	{
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < -20.f)
		{
			m_pTransformCom->Set_Vel(0.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(-16.f, 1.f, -16.f));
			CGameInstance::Get_Instance()->PlaySoundW(L"OpenMini.mp3", 4, 1.f);
		}
	}
	else
	{
		m_fRespownPosAcc += 1.f * fTimeDelta;
		if (2.f < m_fRespownPosAcc)
		{
			if ((m_pTransformCom->Get_Vel() > -0.1f) && (m_eCurState == STATE_IDLE || m_eCurState == STATE_MOVE))
				m_vRespownPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			m_fRespownPosAcc = 0.f;
		}
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < -20.f)
		{
			m_pTransformCom->Set_Vel(0.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vRespownPos);
			CGameInstance::Get_Instance()->PlaySoundW(L"OpenMini.mp3", 4, 1.f);
		}
	}

		


	
	SetOnceEndAni();
	

	// TEST
	//if (CGameInstance::Get_Instance()->Key_Down(DIK_M))
		m_pTransformCom->Go_Gravity(fTimeDelta);


	__super::BoxColCom_Tick(m_pTransformCom);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_PLAYER, this);
	if(m_eCurState != STATE_DIE)
		m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLSION_PLAYER, this);
	m_pColliderCom->Add_SphereCollsionGroup(CCollider::COLLSION_PLAYER, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	
}
HRESULT CPlayer::Render()
{
	/*Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;	

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;
	

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	if(m_eCurState != STATE_DIE)
		m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;*/
	
	Set_Billboard();

	_float4x4			WorldMatrix, ViewMatrix, ProjMatrix;
	_float4x4			ViewMatrixInv;

	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);


	m_pShaderCom->Set_RawValue("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	m_pShaderCom->Set_RawValue("g_vCamPosition", &ViewMatrixInv.m[3][0], sizeof(_float4));
	
	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	m_pShaderCom->Set_Texture("g_Texture", m_pAnimatorCom->Find_Component()->Get_Tex(m_pAnimatorCom->Get_AnimCount()));

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	if(m_eCurState != STATE_DIE)
		m_pVIBufferCom->Render();

	m_pShaderCom->End();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;


	if (CGameInstance::Get_Instance()->Key_Down(DIK_0))
	{
		if (temp)
			temp = false;
		else
			temp = true;
	}

	if (temp)
		__super::BoxColCom_Render(m_pTransformCom);
	


	/*_float3 pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	wchar_t PosX[10];
	_itow_s(pos.x, PosX, 10);

	wchar_t PosY[10];
	_itow_s(pos.y, PosY, 10);

	wchar_t PosZ[10];
	_itow_s(pos.z, PosZ, 10);

	RECT X;
	SetRect(&X, 800, 180, 0, 0);
	m_Font->DrawText(NULL, PosX, -1, &X, DT_NOCLIP, D3DXCOLOR(0.f, 255.f, 0.0f, 1.0f));

	RECT Y;
	SetRect(&Y, 820, 180, 0, 0);
	m_Font->DrawText(NULL, PosY, -1, &Y, DT_NOCLIP, D3DXCOLOR(0.f, 255.f, 0.0f, 1.0f));

	RECT Z;
	SetRect(&Z, 840, 180, 0, 0);
	m_Font->DrawText(NULL, PosZ, -1, &Z, DT_NOCLIP, D3DXCOLOR(0.f, 255.f, 0.0f, 1.0f));*/




	return S_OK;
}




void CPlayer::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;

	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();

	if (m_eCurState == STATE_JUMP)
	{
		// *�߷� �ڵ�
		// Y�� ���� �ݴϴ�.
		m_pTransformCom->Set_Vel(m_fJumpPower);
	}
	else if(m_eCurState == STATE_DJUMP)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CSolunaSlashEffectB::SOLUNAEFFECTBDESC SolunaDECS;
		SolunaDECS.eDir = m_eDir;
		pGameInstance->PlaySoundW(L"DoubleJump.wav", 20, 1.f);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_WarriorReef"), LEVEL_STATIC, TEXT("Layer_Skill"), &SolunaDECS);

		Safe_Release(pGameInstance);

		m_pTransformCom->Set_Vel(3.0f);
	}
}
void CPlayer::SetState(STATE eState)
{
	if (m_eCurState == eState)
		return;

	m_eCurState = eState;
	SetAni();

	if (m_eCurState == STATE_JUMP)
	{
		// *�߷� �ڵ�
		// Y�� ���� �ݴϴ�.
		m_pTransformCom->Set_Vel(m_fJumpPower);
	}
	else if (m_eCurState == STATE_DJUMP)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CSolunaSlashEffectB::SOLUNAEFFECTBDESC SolunaDECS;
		SolunaDECS.eDir = m_eDir;
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_WarriorReef"), LEVEL_STATIC, TEXT("Layer_Skill"), &SolunaDECS);

		Safe_Release(pGameInstance);

		m_pTransformCom->Set_Vel(3.0f);
	}
}
void CPlayer::SetOnceEndAni()
{
	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
	{
		switch (m_eCurState)
		{
		case Client::CCreature::STATE_IDLE:
			break;
		case Client::CCreature::STATE_MOVE:
			break;
		case Client::CCreature::STATE_JUMP:
			break;
		case Client::CCreature::STATE_ATTACK:
			SetState(STATE_IDLE, m_eDir);
			break;
		case Client::CCreature::STATE_DASH:
			break;
		case Client::CCreature::STATE_HIT:
			break;
		case Client::CCreature::STATE_CHASE:
			break;
		case Client::CCreature::STATE_DIE:
			break;
		}
	}
}
void CPlayer::SetRespownPos(_float3 RePos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, RePos);
	m_vRespownPos = RePos;
}
void CPlayer::SetAni()
{
	switch (m_eCurState)
	{
	case CPlayer::STATE_IDLE:
	{
		switch (m_eDir)
		{
		case Client::CPlayer::DIR_L:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_L"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_R:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_R"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_U:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_U"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_D:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_D"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_LU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_LU"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_RU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_RU"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_LD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_LD"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_RD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Idle_RD"), 0.05f, CAnimator::STATE_LOOF);
			break;
		}
	}
		break;
	case CPlayer::STATE_MOVE:
	{
		switch (m_eDir)
		{
		case Client::CPlayer::DIR_L:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_L"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_R:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_R"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_U:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_U"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_D:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_D"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_LU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_LU"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_RU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_RU"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_LD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_LD"), 0.05f, CAnimator::STATE_LOOF);
			break;
		case Client::CPlayer::DIR_RD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Move_RD"), 0.05f, CAnimator::STATE_LOOF);
			break;
		}
	}
		break;
	case CPlayer::STATE_JUMP:
	{
		switch (m_eDir)
		{
		case Client::CPlayer::DIR_L:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_L"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_R:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_R"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_U:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_U"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_D:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_D"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_LU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_LU"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_RU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_RU"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_LD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_LD"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_RD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Jump_RD"), 0.05f, CAnimator::STATE_ONCE);
			break;
		}
	}
		break;
	case CPlayer::STATE_ATTACK:
	{
		switch (m_eDir)
		{
		case Client::CPlayer::DIR_L:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_L"), 0.04f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_R:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_R"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_U:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_U"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_D:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_D"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_LU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_LU"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_RU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_RU"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_LD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_LD"), 0.05f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_RD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_RD"), 0.05f, CAnimator::STATE_ONCE);
			break;
		}
	}
	break;
	case CPlayer::STATE_DASH:
	{
		switch (m_eDir)
		{
		case Client::CPlayer::DIR_L:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_L"), 0.01f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_R:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_R"), 0.01f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_U:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_U"), 0.01f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_D:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_D"), 0.01f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_LU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_LU"), 0.01f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_RU:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_RU"), 0.01f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_LD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_LD"), 0.01f, CAnimator::STATE_ONCE);
			break;
		case Client::CPlayer::DIR_RD:
			m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Player_Attack_RD"), 0.01f, CAnimator::STATE_ONCE);
			break;
		}
	}


		break;
	}
}
void CPlayer::SetShadow(LEVEL eLevel, _float fScale)
{
	// Test �׸���
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CShadow::SHADOWDESC ShadowDesc;
	ShadowDesc.fScale = fScale * 0.5f;

	CGameObject* pShadow = nullptr;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Shadow"), eLevel, TEXT("Layer_Shadow"), &pShadow, &ShadowDesc)))
		return;
	m_pShadow = (CShadow*)pShadow;
	//Safe_AddRef(m_pShadow);

	Safe_Release(pGameInstance);
}



void CPlayer::GetKeyInput(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (_bPushBlock)
	{
		CTransform::TRANSFORMDESC IdleDesc;
		IdleDesc.fSpeedPerSec = 0.2f;
		IdleDesc.fRotationPerSec = D3DXToRadian(90.0f);
		m_pTransformCom->Set_TransformDesc(IdleDesc);
	}
	else
	{
		CTransform::TRANSFORMDESC IdleDesc;
		IdleDesc.fSpeedPerSec = 4.f;
		IdleDesc.fRotationPerSec = D3DXToRadian(90.0f);
		m_pTransformCom->Set_TransformDesc(IdleDesc);
	}
	_bPushBlock = false;

	if (GetKeyState(VK_UP) & 0x8000)
	{
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_RU);
			m_pTransformCom->Go_RU(fTimeDelta);
		}
		else if (GetKeyState(VK_LEFT) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_LU);
			m_pTransformCom->Go_LU(fTimeDelta);
		}
		else
		{
			SetState(STATE_MOVE, DIR_U);
			m_pTransformCom->Go_U(fTimeDelta);
		}
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_RD);
			m_pTransformCom->Go_RD(fTimeDelta);
		}
		else if (GetKeyState(VK_LEFT) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_LD);
			m_pTransformCom->Go_LD(fTimeDelta);
		}
		else
		{
			SetState(STATE_MOVE, DIR_D);
			m_pTransformCom->Go_D(fTimeDelta);
		}
	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (GetKeyState(VK_UP) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_LU);
			m_pTransformCom->Go_LU(fTimeDelta);

		}
		else if (GetKeyState(VK_DOWN) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_LD);
			m_pTransformCom->Go_LD(fTimeDelta);
		}
		else
		{
			SetState(STATE_MOVE, DIR_L);
			m_pTransformCom->Go_L(fTimeDelta);
		}
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		if (GetKeyState(VK_UP) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_RU);
			m_pTransformCom->Go_RU(fTimeDelta);
		}
		else if (GetKeyState(VK_DOWN) & 0x8000)
		{
			SetState(STATE_MOVE, DIR_RD);
			m_pTransformCom->Go_RD(fTimeDelta);
		}
		else
		{
			SetState(STATE_MOVE, DIR_R);
			m_pTransformCom->Go_R(fTimeDelta);
		}
	}
	else
	{
		SetState(STATE_IDLE, m_eDir);
	}
	if (GetKeyState(VK_LCONTROL)&0X8000)
	{
		SetState(STATE_ATTACK, m_eDir);
	}


	if (GetKeyState('X') & 0x8000)
	{
		SetState(STATE_JUMP, m_eDir);
		pGameInstance->PlaySoundW(L"Jump.mp3", 20, 1.f);
	}

	/*if (GetKeyState('C') & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CSolunaSlashEffectB::SOLUNAEFFECTBDESC SolunaDECS;
		SolunaDECS.eDir = m_eDir;
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_WarriorReef"), LEVEL_STATIC, TEXT("Layer_Skill"), &SolunaDECS);
		SetState(STATE_DASH, m_eDir);
		Safe_Release(pGameInstance);
	}*/

	if (GetKeyState('C') & 0x8000)
	{
		CSolunaSlashEffectB::SOLUNAEFFECTBDESC SolunaDECS;
		SolunaDECS.eDir = m_eDir;
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SolunaSlash_EffectA"), LEVEL_STATIC, TEXT("Layer_Skill"), &SolunaDECS);
		pGameInstance->PlaySoundW(L"SolunaSlash.mp3", 20, 1.f);
		//pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SolunaSlash_EffectB"), LEVEL_STATIC, TEXT("Layer_Skill"), &SolunaDECS);
		SetState(STATE_DASH, m_eDir);
	}

	if (pGameInstance->Key_Down(DIK_A))
	{
		pGameInstance->PlaySound(L"ReefAttack.mp3", 20, 1.f);

		CReefAttack::REEFATTACKDESC ReefAttackDESC;
		ReefAttackDESC.eDir = m_eDir;

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ReefAttack"), LEVEL_STATIC, TEXT("Layer_Skill"), &ReefAttackDESC);
		SetState(STATE_ATTACK, m_eDir);


		// TEST
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.y += 1.5f;
		// CParticleManager::Get_Instance()->MakeDamageGen(1000, 2000, 4, 0.1f, vPos, 1.f, true);


		// CParticleManager::Get_Instance()->Shot(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	
	}

	if (GetKeyState('S') & 0x8000)
	{
		CCardinalBlastAttack::CARDINALATTACKDESC CardinalAttackDESC;
		CardinalAttackDESC.eDir = m_eDir;
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_CardinalBlast_Attack"), LEVEL_STATIC, TEXT("Layer_Skill"), &CardinalAttackDESC);
		SetState(STATE_ATTACK, m_eDir);
	}

	if (pGameInstance->Key_Down(DIK_D))
	{
		CSunCross::SUNCROSSDESC SunCrossDECS;
		SunCrossDECS.eDir = m_eDir;
		pGameInstance->PlaySound(L"Suncross.mp3", 20, 1.f);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SunCross"), LEVEL_STATIC, TEXT("Layer_Skill"), &SunCrossDECS);
		SetState(STATE_ATTACK, m_eDir);
		// TEST
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.y += 1.5f;
		// CParticleManager::Get_Instance()->MakeDamageGen(1000, 2000, 4, 0.05f, vPos, 1.3f, false);

	}

	if (GetKeyState('F') & 0x8000)
	{

		CSpearPulling::SPEARPULLINGDESC SpearPullingDECS;
		SpearPullingDECS.eDir = m_eDir;
		pGameInstance->PlaySoundW(L"SpearPulling.mp3", 20, 1.f);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SpearPulling"), LEVEL_STATIC, TEXT("Layer_Skill"), &SpearPullingDECS);
		SetState(STATE_ATTACK, m_eDir);
	}

	if (GetKeyState('G') & 0x8000)
	{
		CChasingShotAttack::CHASINGATTACKDESC ChasingShotDECS;
		ChasingShotDECS.eDir = m_eDir;
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_ChasingShot_Attack"), LEVEL_STATIC, TEXT("Layer_Skill"), &ChasingShotDECS);
		SetState(STATE_ATTACK, m_eDir);
	
	}

	if (pGameInstance->Key_Down(DIK_Q))
	{
		CBeastEffect::BEASTEFFECTDESC BeastDECS;
		BeastDECS.eDir = m_eDir;

		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Beast_Effect"), LEVEL_STATIC, TEXT("Layer_Skill"), &BeastDECS);
		SetState(STATE_ATTACK, m_eDir);

	}

	if (pGameInstance->Key_Down(DIK_W))
	{	
		CUIManager::Get_Instance()->Set_StartMove(true);

	}

	if (pGameInstance->Key_Down(DIK_R))
	{
		CUIManager::Get_Instance()->Set_EndMove(true);

	}

	if (pGameInstance->Key_Down(DIK_NUMPAD4))
	{
		CInvenManager::Get_Instance()->Get_ItemInfo(L"RedPortionInfo", CInvenManager::TYPE_CONSUM)->Set_NowNum(99);
	}

	if (pGameInstance->Key_Down(DIK_NUMPAD5))
	{
		CInvenManager::Get_Instance()->Get_ItemInfo(L"BluePortionInfo", CInvenManager::TYPE_CONSUM)->Set_NowNum(99);

	}

	if (pGameInstance->Key_Down(DIK_NUMPAD6))
	{
		CInvenManager::Get_Instance()->Get_ItemInfo(L"TeleportInfo", CInvenManager::TYPE_CONSUM)->Set_NowNum(99);

	}

	Safe_Release(pGameInstance);

	
}
void CPlayer::GetJumpKeyInput(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (GetKeyState(VK_UP) & 0x8000)
	{
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			m_pTransformCom->Go_RU(fTimeDelta);
		}
		else if (GetKeyState(VK_LEFT) & 0x8000)
		{
			m_pTransformCom->Go_LU(fTimeDelta);
		}
		else
		{
			m_pTransformCom->Go_U(fTimeDelta);
		}
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			m_pTransformCom->Go_RD(fTimeDelta);
		}
		else if (GetKeyState(VK_LEFT) & 0x8000)
		{
			m_pTransformCom->Go_LD(fTimeDelta);
		}
		else
		{
			m_pTransformCom->Go_D(fTimeDelta);
		}
	}
	else if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (GetKeyState(VK_UP) & 0x8000)
		{
			m_pTransformCom->Go_LU(fTimeDelta);

		}
		else if (GetKeyState(VK_DOWN) & 0x8000)
		{
			m_pTransformCom->Go_LD(fTimeDelta);
		}
		else
		{
			m_pTransformCom->Go_L(fTimeDelta);
		}
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		if (GetKeyState(VK_UP) & 0x8000)
		{
			m_pTransformCom->Go_RU(fTimeDelta);
		}
		else if (GetKeyState(VK_DOWN) & 0x8000)
		{
			m_pTransformCom->Go_RD(fTimeDelta);
		}
		else
		{
			m_pTransformCom->Go_R(fTimeDelta);
		}
	}

	if (pGameInstance->Key_Down(DIK_X))
	{
		SetState(STATE_DJUMP, m_eDir);
		CParticleManager::Get_Instance()->BackShot(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	}


	if (pGameInstance->Key_Down(DIK_U))
	{
		CQuestManager::Get_Instance()->Hunting(TEXT("GAS"));
	}

	Safe_Release(pGameInstance);

}





void CPlayer::Idle(_float fTimeDelta)
{
	CTransform::TRANSFORMDESC IdleDesc;
	IdleDesc.fSpeedPerSec = 4.f;
	IdleDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_pTransformCom->Set_TransformDesc(IdleDesc);
	m_fJumpPower = m_fOriJumpPower;
}
void CPlayer::Dash(_float fTimeDelta)
{
	CTransform::TRANSFORMDESC DashDesc;
	DashDesc.fSpeedPerSec = 20.f;
	DashDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_pTransformCom->Set_TransformDesc(DashDesc);
	m_fJumpPower = m_fOriJumpPower;


	m_fDashAcc += 1.f * fTimeDelta;
	if (0.18f < m_fDashAcc)
	{
		SetState(STATE_IDLE, m_eDir);
		m_fDashAcc = 0.f;
	}

	switch (m_eDir)
		{
		case Client::CPlayer::DIR_L:
			m_pTransformCom->Go_L(fTimeDelta);
			break;
		case Client::CPlayer::DIR_R:
			m_pTransformCom->Go_R(fTimeDelta);
			break;
		case Client::CPlayer::DIR_U:
			m_pTransformCom->Go_U(fTimeDelta);
			break;
		case Client::CPlayer::DIR_D:
			m_pTransformCom->Go_D(fTimeDelta);
			break;
		case Client::CPlayer::DIR_LU:
			m_pTransformCom->Go_LU(fTimeDelta);
			break;
		case Client::CPlayer::DIR_RU:
			m_pTransformCom->Go_RU(fTimeDelta);
			break;
		case Client::CPlayer::DIR_LD:
			m_pTransformCom->Go_LD(fTimeDelta);
			break;
		case Client::CPlayer::DIR_RD:
			m_pTransformCom->Go_RD(fTimeDelta);
			break;
		}
}
void CPlayer::Jump(_float fTimeDelta)
{
	CTransform::TRANSFORMDESC IdleDesc;
	IdleDesc.fSpeedPerSec = 2.f;
	IdleDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_pTransformCom->Set_TransformDesc(IdleDesc);

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
	}
}
void CPlayer::DoubleJump(_float fTimeDelta)
{
	CTransform::TRANSFORMDESC IdleDesc;
	IdleDesc.fSpeedPerSec = 8.f;
	IdleDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_pTransformCom->Set_TransformDesc(IdleDesc);

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
	}


}

void CPlayer::Die(_float fTimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	CCreature::CRETUREDESC vTomb;

	vTomb.vPos = vPlayerPos + _float3(0.f,0.f,-1.f);

	if (!m_bDie)
	{
		CParticleManager::Get_Instance()->MakeDie(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_TombUI"), LEVEL_STATIC, TEXT("Layer_UI"));
		m_bDie = true;
	}


	Safe_Release(pGameInstance);
}


void CPlayer::Particle(_float fTimeDelta)
{

	switch (m_eCurState)
	{
	case STATE_MOVE:
	{
			_fParticleMoveTimeAcc += 1.f * fTimeDelta;
	if (0.3f < _fParticleMoveTimeAcc)
	{
		CParticleManager::Get_Instance()->Player_Walk(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_fParticleMoveTimeAcc = 0.f;
	}
	}
		break;
	default:
		break;
	}







}

void CPlayer::GetKeyCheate(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (CGameInstance::Get_Instance()->Key_Down(DIK_1))
	{
		CItem::ITEMINFO SlimeItem;
		SlimeItem.eType = CInvenManager::TYPE_STUFF;
		SlimeItem.iTextNum = 0;
		SlimeItem.pTag = L"BlueSnailInfo";
		SlimeItem.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &SlimeItem);
	}
	if (CGameInstance::Get_Instance()->Key_Down(DIK_2))
	{
		CItem::ITEMINFO SlimeItem;
		SlimeItem.eType = CInvenManager::TYPE_STUFF;
		SlimeItem.iTextNum = 1;
		SlimeItem.pTag = L"RibbonPigInfo";
		SlimeItem.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &SlimeItem);
	}
	if (CGameInstance::Get_Instance()->Key_Down(DIK_3))
	{
		CItem::ITEMINFO SlimeItem;
		SlimeItem.eType = CInvenManager::TYPE_STUFF;
		SlimeItem.iTextNum = 2;
		SlimeItem.pTag = L"SlimeInfo";
		SlimeItem.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &SlimeItem);
	}
	if (CGameInstance::Get_Instance()->Key_Down(DIK_4))
	{
		CItem::ITEMINFO SlimeItem;
		SlimeItem.eType = CInvenManager::TYPE_STUFF;
		SlimeItem.iTextNum = 3;
		SlimeItem.pTag = L"GreenMushroomInfo";
		SlimeItem.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &SlimeItem);
	}
	if (CGameInstance::Get_Instance()->Key_Down(DIK_5))
	{
		CItem::ITEMINFO SlimeItem;
		SlimeItem.eType = CInvenManager::TYPE_STUFF;
		SlimeItem.iTextNum = 4;
		SlimeItem.pTag = L"GoStumpInfo";
		SlimeItem.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &SlimeItem);
	}


	if (CGameInstance::Get_Instance()->Key_Down(DIK_7))
	{
		CItem::ITEMINFO RedPortion;
		RedPortion.eType = CInvenManager::TYPE_CONSUM;
		RedPortion.iTextNum = 0;
		RedPortion.pTag = L"RedPortionInfo";
		RedPortion.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &RedPortion);
	}

	if (CGameInstance::Get_Instance()->Key_Down(DIK_8))
	{
		CItem::ITEMINFO BluePortion;
		BluePortion.eType = CInvenManager::TYPE_CONSUM;
		BluePortion.iTextNum = 1;
		BluePortion.pTag = L"BluePortionInfo";
		BluePortion.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &BluePortion);
	}

	if (CGameInstance::Get_Instance()->Key_Down(DIK_9))
	{
		CItem::ITEMINFO Teleport;
		Teleport.eType = CInvenManager::TYPE_CONSUM;
		Teleport.iTextNum = 2;
		Teleport.pTag = L"TeleportInfo";
		Teleport.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Item"), LEVEL_GAMEPLAY, TEXT("Layer_Item"), &Teleport);

	}

	Safe_Release(pGameInstance);
}

void CPlayer::CheckAttakedTime(_float fTimeDelta)
{
	if (!m_bAttaked)
		return;

	m_fAttakedTimeAcc += fTimeDelta;

	if (2.f < m_fAttakedTimeAcc)
	{
		m_bAttaked = false;
		m_fAttakedTimeAcc = 0.f;
	}
}

void CPlayer::Shading(_float fTImeDelta)
{
	if (m_bHit)
	{
		m_pShaderCom->Set_RawValue("g_vPlayerColor", _float4{ 255.f,255.f,255.f,1.f }, sizeof(_float4));
		m_fShadingAcc += 1.f * fTImeDelta;
		m_fChange += 1.f * fTImeDelta;

		
		if (m_fChange > 0.1)
		{
			m_bChange = !m_bChange;
			m_fChange = 0.f;
		}

		if(m_bChange)
			m_pShaderCom->Set_RawValue("g_vPlayerColor", _float4{ 255.f,255.f,255.f,1.f }, sizeof(_float4));
		else
			m_pShaderCom->Set_RawValue("g_vPlayerColor", _float4{ 1.f,1.f,1.f,1.f }, sizeof(_float4));
		

		if (m_fShadingAcc > 2)
		{
			m_pShaderCom->Set_RawValue("g_vPlayerColor", _float4{ 1.f,1.f,1.f,1.f }, sizeof(_float4));
			m_bHit = false;
			m_bChange = false;
			m_fShadingAcc = 0.f;
			m_fChange = 0.f;
		}
	}
}
void CPlayer::Player_Cut()
{
	CGameInstance::Get_Instance()->PlaySound(L"Tomb.wav", 13, 1.f);
	SetState(STATE_DIE, DIR_END);
	m_bDie = false;
	m_bDieing = true;
}

HRESULT CPlayer::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}
HRESULT CPlayer::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;
}



CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPlayer::Damaged(CGameObject * pOther)
{
	CTransform* pTran = (CTransform*)pOther->Get_ComponentPtr(TEXT("Com_Transform"));
	_float3 vInvDir = -1.f * m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	SetKnockBack(1.5f, 3.f, vInvDir);
	m_bAttaked = true;
	CUIManager::Get_Instance()->Set_PlayerHp(pOther->Get_Damage());
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos.y += 1.5f;
	m_bHit = true;
	m_bChange = true;
	CParticleManager::Get_Instance()->MakeDamageGen(500, 2000, 1, 0.05f, vPos, 1.6f, true, CDamage::DAMAGE_PLAYERATTAKED);
}


void CPlayer::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Cube")
	{
		if (m_eCurState == STATE_JUMP || m_eCurState == STATE_DJUMP || m_eCurState == STATE_KNOCKBACK)
		{
			if (Get_PushedY())
			{
				SetState(STATE_IDLE, m_eDir);
				CParticleManager::Get_Instance()->Player_Lend(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			}
		}
		
	}

	if (pOther->Get_Tag() == "Tag_PushCube")
		_bPushBlock = true;
		


	if (pOther->Get_Tag() == "Tag_Item")
	{

		if (GetKeyState('Z') & 0x8000)
		{
			//�޴������ؼ� ������ ���� �߰� 
			//CInvenManager* m_pInvenMgr = CInvenManager::Get_Instance();
			//Safe_AddRef(m_pInvenMgr);
			//m_pInvenMgr->AddItemNum();
		}

	}
	if (pOther->Get_Tag() == "Tag_Monster" && !m_bAttaked)
	{
		if (m_eCurState != STATE_DIE)
		{
			Damaged(pOther);
		}
	}

	if (pOther->Get_Tag() == "Tag_MonsterSkill" && !m_bAttaked)
	{
		if (m_eCurState != STATE_DIE)
		{
			Damaged(pOther);
		}
	}
	if (!m_bDieing)
	{
		if (CUIManager::Get_Instance()->Get_PlayerHp() == 21)
			Player_Cut();	
	}
}

void CPlayer::OnLay(_float3 vOutDis)
{
	__super::OnLay(vOutDis);


}





void CPlayer::Free()
{
	__super::Free();

	//Safe_Release(m_pShadow);
}


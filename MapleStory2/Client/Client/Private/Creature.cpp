#include "stdafx.h"
#include "..\Public\Creature.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Shadow.h"

CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_eCurState(STATE_IDLE)
{
}
CCreature::CCreature(const CCreature & rhs)
	: CGameObject(rhs)
	, m_eCurState(STATE_IDLE)
{
}




HRESULT CCreature::Initialize_Prototype()
{
	return S_OK;
}
HRESULT CCreature::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}




HRESULT CCreature::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"), TEXT("Com_Animator"), (CComponent**)&m_pAnimatorCom)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;

	

	return S_OK;
}




void CCreature::Tick(_float fTimeDelta)
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
		break;
	}
	
}
void CCreature::LateTick(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		SetState(STATE_IDLE, m_eDir);

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}
HRESULT CCreature::Render()
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

	return S_OK;
}




void CCreature::SetState(STATE eState, DIR eDir)
{
	if (m_eCurState == eState && m_eDir == eDir)
		return;
	
	m_eCurState = eState;
	m_eDir = eDir;
	SetAni();
}
void CCreature::SetState(STATE eState)
{
	if (m_eCurState == eState)
		return;

	m_eCurState = eState;
	SetAni();
}
void CCreature::Set_Dead()
{
	m_bDead = true;

	if (nullptr != m_pShadow)
		m_pShadow->Set_Dead();
}
void CCreature::SetAni()
{
	switch (m_eCurState)
	{
	case CCreature::STATE_IDLE:
		break;
	case CCreature::STATE_MOVE:
		break;
	case CCreature::STATE_JUMP:
		break;
	case CCreature::STATE_ATTACK:
		break;
	}
}

void CCreature::Set_Billboard()
{
	_float3 fScale = m_pTransformCom->Get_Scaled();

	m_vLookTemp = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */ 
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * fScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * fScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * fScale.z);
}

void CCreature::SetKnockBack(_float fPowerY, _float fPowerX, _float3 vDir)
{
	m_fKnockPowerX = fPowerX;
	m_fKnockPowerY = fPowerY;
	m_vKnockDir = vDir;
	m_pTransformCom->Set_Vel(m_fKnockPowerY);
	SetState(STATE_KNOCKBACK, m_eDir);
}

void CCreature::TickKnockBack(_float fTimeDelta)
{
	m_pTransformCom->Go_DirForce(m_vKnockDir, m_fKnockPowerX, fTimeDelta);
}


void CCreature::SetShadow(LEVEL eLevel, _float fScale)
{
	// Test 그림자
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CShadow::SHADOWDESC ShadowDesc;
	ShadowDesc.fScale = fScale;

	CGameObject* pShadow = nullptr;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_Shadow"), eLevel, TEXT("Layer_Shadow"), &pShadow, &ShadowDesc)))
		return;
	m_pShadow = (CShadow*)pShadow;
	//Safe_AddRef(m_pShadow);

	Safe_Release(pGameInstance);
}



HRESULT CCreature::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}
HRESULT CCreature::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;
}





CCreature * CCreature::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCreature*		pInstance = new CCreature(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCreature"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CCreature::Clone(void* pArg)
{
	CCreature*		pInstance = new CCreature(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCreature"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CCreature::OnLay(_float3 vOutDis)
{
	if (nullptr == m_pShadow)
		return;

	vOutDis.y += 0.01f;
	vOutDis.z -= 0.2f;
	CTransform* pShadowTrans = (CTransform*)m_pShadow->Get_ComponentPtr(TEXT("Com_Transform"));
	pShadowTrans->Set_State(CTransform::STATE_POSITION, vOutDis);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float y = vPos.y - vOutDis.y;
	_float fOriScale = m_pShadow->Get_OriScale();
	pShadowTrans->Set_Scaled(fOriScale - y * 0.1f);
}



void CCreature::Free()
{
	__super::Free();


	Safe_Release(m_pAnimatorCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
}


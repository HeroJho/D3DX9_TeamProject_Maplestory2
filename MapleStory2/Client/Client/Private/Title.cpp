#include "stdafx.h"
#include "..\Public\Title.h"

#include "GameInstance.h"
#include "ToolManager.h"


CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CTitle::CTitle(const CTitle& rhs)
	: CUI(rhs)
{
}


HRESULT CTitle::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CTitle::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 500.f;
	m_UIInfo.fSizeY = 500.f;
	m_UIInfo.fX = g_iWinSizeX*0.5f;
	m_UIInfo.fY = g_iWinSizeY*0.5f - 200.f;
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_iTexturenum = 0;

	__super::Initialize(pArg);

	m_bRender = false;


	return S_OK;
}



void CTitle::Tick(_float fTimeDelta)
{
	if (!m_bStart)
		return;

	m_fTimeAcc += fTimeDelta;

	if (2.5f < m_fTimeAcc)
	{
		m_bRender = true;
		m_bStart = false;
		m_fTimeAcc = 0.f;
	}


}

void CTitle::LateTick(_float fTimeDelta)
{
	if (!m_bRender)
		return;

	if (m_pAnimatorCom->Get_AniInfo().eMode == CAnimator::STATE_ONCEEND)
		m_bRender = false;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CTitle::Render()
{
	if (!m_bRender)
		return S_OK;


	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	_float fDF = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (FAILED(m_pAnimatorCom->Play_Ani(1.f * fDF)))
		return E_FAIL;

	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();




	return S_OK;
}



void CTitle::PlayTitleCurMap(LEVEL eLevel)
{
	m_bStart = true;
	m_bRender = false;
	m_fTimeAcc = 0.f;

	switch (eLevel)
	{
	case Client::LEVEL_GAMEPLAY:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_Henesys"), 0.1f, CAnimator::STATE_ONCE);
		break;

	case Client::LEVEL_HENESYS:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_HenesysHunting"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_ELENYAENTER:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_ElenyaEnter"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_ELENYA:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_Elenya"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_DHENESYS:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_Henesys"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_DHENESYSHUNTING:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_HenesysHunting"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_DELENYAENTER:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_ElenyaEnter"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_DELENYA:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_Elenya"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_DARKMAGEJUMP:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_ElderStan_Idle"), 0.1f, CAnimator::STATE_ONCE);

		break;
	case Client::LEVEL_DARKMAGEENTER:
		m_pAnimatorCom->Set_AniInfo(TEXT("Prototype_Component_Texture_Tile_EndOfWorl"), 0.1f, CAnimator::STATE_ONCE);

		break;
	default:
		m_bStart = false;
		break;

	}
}



HRESULT CTitle::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Animator"), TEXT("Com_Animator"), (CComponent**)&m_pAnimatorCom)))
		return E_FAIL;


	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Henesys"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_HenesysHunting"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_ElenyaEnter"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Elenya"), nullptr);
	m_pAnimatorCom->Create_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_EndOfWorl"), nullptr);




	return S_OK;
}

CTitle* CTitle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTitle*		pInstance = new CTitle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCutScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTitle::Clone(void * pArg)
{
	CTitle*		pInstance = new CTitle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCutScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTitle::Free()
{
	__super::Free();

	Safe_Release(m_pAnimatorCom);
}

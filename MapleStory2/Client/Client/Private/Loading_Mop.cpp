#include "stdafx.h"
#include "..\Public\Loading_Mop.h"
#include "GameInstance.h"
#include "SkillManager.h"


CLoading_Mop::CLoading_Mop(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CLoading_Mop::CLoading_Mop(const CLoading_Mop& rhs)
	: CUI(rhs)
{
}


HRESULT CLoading_Mop::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CLoading_Mop::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_UIInfo.fSizeX = 150;
	m_UIInfo.fSizeY = 150;
	m_UIInfo.fX = g_iWinSizeX*0.5f;
	m_UIInfo.fY = g_iWinSizeY*0.5f - 50.f;
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));
	m_iTexturenum = 0;

	__super::Initialize(pArg);


	m_fMaxCount = 3;
	m_fTime = 0.3f;
	m_bRender = true;

	return S_OK;
}

void CLoading_Mop::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTime < m_fTimeAcc)
	{
		++m_fCount;
		if (m_fCount >= m_fMaxCount)
		{
			m_fCount = 0.f;
			m_fTimeAcc = 0.f;
			return;
		}

		m_fTimeAcc = 0.f;
	}

}

void CLoading_Mop::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLoading_Mop::Render()
{
	if (!m_bRender)
		return S_OK;



	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);


	switch (m_eImg)
	{
		case IMG_MUSHROOM:
		{
			if (FAILED(m_pTextureCom->Bind_Texture(m_fCount)))
				return E_FAIL;
		}
			break;
		case IMG_SLIME:
		{
			if (FAILED(m_pSlimeTextureCom->Bind_Texture(m_fCount)))
				return E_FAIL;
		}
			break;
	}



	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();
	return S_OK;
}



HRESULT CLoading_Mop::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OrangeMushroom_Move"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Slime_Move"), TEXT("Com_Texture1"), (CComponent**)&m_pSlimeTextureCom)))
		return E_FAIL;


	

	return S_OK;
}


CLoading_Mop* CLoading_Mop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_Mop*		pInstance = new CLoading_Mop(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : Loading_Mop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLoading_Mop::Clone(void * pArg)
{
	CLoading_Mop*		pInstance = new CLoading_Mop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : Loading_Mop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Mop::Free()
{
	__super::Free();

	Safe_Release(m_pSlimeTextureCom);
}

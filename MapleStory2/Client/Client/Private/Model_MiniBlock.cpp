#include "stdafx.h"
#include "..\Public\Model_MiniBlock.h"

#include "GameInstance.h"
#include "ParticleManager.h"
#include "ToolManager.h"

CModel_MiniBlock::CModel_MiniBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CModel_MiniBlock::CModel_MiniBlock(const CModel_MiniBlock & rhs)
	: CGameObject(rhs)
{
}


HRESULT CModel_MiniBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModel_MiniBlock::Initialize(void * pArg)
{

	if (nullptr == pArg)
		return E_FAIL;

	m_sTag = "Tag_MiniBlock";

	m_Desc = *(MINIBLOCKDESC*)pArg;



	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Desc.vPos);
	m_pTransformCom->Set_Scaled(0.15f);
	//m_pTransformCom->RotationThree(
	//	_float3(1.f, 0.f, 0.f), pVoxDesc->vRotationAix.x,
	//	_float3(0.f, 1.f, 0.f), pVoxDesc->vRotationAix.y + 45.f,
	//	_float3(0.f, 0.f, 1.f), pVoxDesc->vRotationAix.z);

	return S_OK;
}

void CModel_MiniBlock::Tick(_float fTimeDelta)
{


	CGameObject* pPlayer = CToolManager::Get_Instance()->GetPlayer();
	CTransform* pPTran = (CTransform*)pPlayer->Get_ComponentPtr(TEXT("Com_Transform"));
	_float3 vPPos = pPTran->Get_State(CTransform::STATE_POSITION);


	if (m_bPicked)
	{
		if (CToolManager::Get_Instance()->Get_MiniBingo())
		{
			Set_Dead();
			CToolManager::Get_Instance()->Set_MiniBingo(false);
		}
			


		_float3 vPos = vPPos;
		vPos.y += 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}


	if (vPPos.y < -10.f)
	{
		Set_Dead();
	}

}

void CModel_MiniBlock::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Go_Gravity(fTimeDelta);
	__super::BoxColCom_Tick(m_pTransformCom);

	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_MODEL, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CModel_MiniBlock::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	//__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}

HRESULT CModel_MiniBlock::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	return S_OK;
}

HRESULT CModel_MiniBlock::Reset_RenderState()
{


	return S_OK;
}

HRESULT CModel_MiniBlock::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 0.3f, 0.3f, 0.3f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	switch (m_Desc.etype)
	{
	case MINI_RED:
		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Model_MiniRed"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	case MINI_ORRANGE:
		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Model_MiniOrrange"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	case MINI_YERROW:
		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Model_MiniYerrow"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
		break;
	}


	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}


void CModel_MiniBlock::Collision(CGameObject * pOther)
{
	if (pOther->Get_Tag() == "Tag_Cube")
	{
		int i = 0;
		if (!m_bLend)
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			CParticleManager::Get_Instance()->DieModel_Lend(vPos);
			m_bLend = true;
		}
	}

	if (pOther->Get_Tag() == "Tag_Player" && !m_bPicked)
	{
		CModel_MiniBlock::MINICOLOR eColor = CToolManager::Get_Instance()->Get_MiniPicked();
		// 블럭을 들고 있냐
		
		if (eColor == CModel_MiniBlock::MINI_END)
		{// 안들고 있다면 들기
			CToolManager::Get_Instance()->Set_MiniPicked(m_Desc.etype);
			m_bPicked = true;
			CGameInstance::Get_Instance()->PlaySoundW(L"GetMini.mp3", 3, 1.f);
		}
		else
		{// 들고 있다면 짝 확인

			if (eColor == m_Desc.etype)
			{		// 맞다면 다음 블럭 이동
				CToolManager::Get_Instance()->Set_MiniBingo(true);
				Go_DestMini();
				Set_Dead();
				CGameInstance::Get_Instance()->PlaySoundW(L"Clear.mp3", 3, 1.f);
			}
			else
			{		// 틀렸다면 너빽
				CCreature* pPlayer = (CCreature*)pOther;
				pPlayer->SetKnockBack(10.f, 10.f, _float3{-1.f, 0.f, 0.f});
				CGameInstance::Get_Instance()->PlaySoundW(L"FailSound.mp3", 3, 1.f);
			}

		}


	}
}

void CModel_MiniBlock::Go_DestMini()
{
	_float4x4 RotationMatrix;
	D3DXMatrixIdentity(&RotationMatrix);
	D3DXMatrixRotationAxis(&RotationMatrix, &_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(45.f));

	_float3 vTemp;
	if (CModel_MiniBlock::MINI_RED == m_Desc.etype)
	{
		vTemp = _float3{ 1.f, 2.f, -1.f - 15.f };
		CTransform* pTransform = (CTransform*)CToolManager::Get_Instance()->GetPlayer()->Get_ComponentPtr(TEXT("Com_Transform"));
		D3DXVec3TransformCoord(&vTemp, &vTemp, &RotationMatrix);
		pTransform->Set_State(CTransform::STATE_POSITION, vTemp);
	}
		
	else if (CModel_MiniBlock::MINI_ORRANGE == m_Desc.etype)
	{
		vTemp = _float3{ 2.f, 2.f, -1.f - 15.f };
		CTransform* pTransform = (CTransform*)CToolManager::Get_Instance()->GetPlayer()->Get_ComponentPtr(TEXT("Com_Transform"));
		D3DXVec3TransformCoord(&vTemp, &vTemp, &RotationMatrix);
		pTransform->Set_State(CTransform::STATE_POSITION, vTemp);
	}
		
	else if (CModel_MiniBlock::MINI_YERROW == m_Desc.etype)
	{
		vTemp = _float3{ -13.5f, 2.f, -13.5f };
		CTransform* pTransform = (CTransform*)CToolManager::Get_Instance()->GetPlayer()->Get_ComponentPtr(TEXT("Com_Transform"));
		pTransform->Set_State(CTransform::STATE_POSITION, vTemp);
		CToolManager::Get_Instance()->Set_MiniEnd();
	}
		


}



CModel_MiniBlock * CModel_MiniBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CModel_MiniBlock*		pInstance = new CModel_MiniBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CWood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CModel_MiniBlock::Clone(void* pArg)
{
	CModel_MiniBlock*		pInstance = new CModel_MiniBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CWood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel_MiniBlock::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


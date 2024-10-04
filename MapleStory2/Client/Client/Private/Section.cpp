#include "stdafx.h"
#include "..\Public\Section.h"

#include "GameInstance.h"
#include "Cube.h"

CSection::CSection(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSection::CSection(const CSection & rhs)
	: CGameObject(rhs)
{
}


HRESULT CSection::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSection::Initialize(void * pArg)
{
#pragma region 중략
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (nullptr == pArg)
		return E_FAIL;
	m_sTag = "Tag_Section";
	m_pData = (CMap_Manager::CUBEDATA*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pData->vPos);
#pragma endregion

	// 1. Section 그룹에 넣어 큐브와 충돌처리
	m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLSION_SECTION, this);
	__super::BoxColCom_Tick(m_pTransformCom);

	return S_OK;
}

void CSection::Tick(_float fTimeDelta)
{
	list<CGameObject*> s = m_Cubes;
	int i = 0;
}

void CSection::LateTick(_float fTimeDelta)
{
	__super::BoxColCom_Tick(m_pTransformCom);



	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CSection::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (CGameInstance::Get_Instance()->Key_Down(DIK_0))
	{
		if (temp)
			temp = false;
		else
			temp = true;
	}

	if(temp)
		__super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}


HRESULT CSection::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return S_OK;
}

HRESULT CSection::Reset_RenderState()
{


	return S_OK;
}

HRESULT CSection::SetUp_Components()
{

	CBoxCollider::BOXCOLCOMEDESC BoxColDesc;
	ZeroMemory(&BoxColDesc, sizeof(BoxColDesc));
	BoxColDesc.vScale = _float3{ 5.0f, 9.0f, 5.0f };
	BoxColDesc.vPivot = _float3{ 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_BoxColComponent(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"), &BoxColDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
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

CSection * CSection::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSection*		pInstance = new CSection(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSection"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSection::Clone(void* pArg)
{
	CSection*		pInstance = new CSection(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSection"));
		Safe_Release(pInstance);
	}

	return pInstance;
}




void CSection::Collision(CGameObject * pOther)
{
	if ("Tag_Cube" == pOther->Get_Tag())
	{
		CCube* pCube = (CCube*)pOther;
		m_Cubes.push_back(pCube);
		Safe_AddRef(pCube);
	}
}




void CSection::Free()
{
	__super::Free();

	for (auto& pCube : m_Cubes)
		Safe_Release(pCube);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
}


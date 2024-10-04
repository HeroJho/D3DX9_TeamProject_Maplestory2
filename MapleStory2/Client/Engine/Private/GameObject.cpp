#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_fColRad(0.f)
	, m_sTag("")
	, m_bDead(false)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
	, m_fColRad(0.f)
	, m_sTag("")
	, m_bDead(false)
{
	Safe_AddRef(m_pGraphic_Device);
}

CComponent * CGameObject::Get_ComponentPtr(const _tchar * pComponentTag)
{
	CComponent*		pComponent = Find_Component(pComponentTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void * pArg)
{
	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);	

	return S_OK;
}

HRESULT CGameObject::Add_BoxColComponent(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CBoxCollider*			pComponent = (CBoxCollider*)(pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_BoxColliders.push_back(pComponent);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CGameObject::Compute_CamDistance(_float3 vWorldPos)
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vDir = *(_float3*)&ViewMatrix.m[3][0] - vWorldPos;

	m_fCamDistance = D3DXVec3Length(&vDir);

	return S_OK;
}

void CGameObject::BoxColCom_Tick(CTransform* pTrans)
{
	if (m_BoxColliders.empty())
		return;

	for (auto& pBoxColCom : m_BoxColliders)
		pBoxColCom->Tick(pTrans);
}

void CGameObject::BoxColCom_Render(CTransform * pTrans)
{
	if (m_BoxColliders.empty())
		return;

	for (auto& pBoxColCom : m_BoxColliders)
		pBoxColCom->Render(pTrans);
}


CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	for (auto& Com : m_BoxColliders)
		Safe_Release(Com);
	m_BoxColliders.clear();

	Safe_Release(m_pGraphic_Device);
}

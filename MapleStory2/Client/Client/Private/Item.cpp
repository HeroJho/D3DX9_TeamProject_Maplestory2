#include "stdafx.h"
#include "..\Public\Item.h"
#include "GameInstance.h"

#include "ToolManager.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_ItemInfo, pArg, sizeof(ITEMINFO));
	m_sTag = "Tag_Item";
	m_bUP = true;
	m_iDirection = 0;
	switch (m_ItemInfo.eType)
	{
	case CInvenManager::TYPE_EQUIP:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EquipItem"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case CInvenManager::TYPE_CONSUM:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ConsumItem"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case CInvenManager::TYPE_STUFF:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_StuffItem"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	default:
		break;
	}
	m_pTransformCom->Set_Scaled(0.3f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_ItemInfo.vPos);


	m_pTransformCom->Set_Vel(4.f);


	m_vLook = _float3{ 1.f, 0.f, 0.f };
	_float Angle = CGameInstance::Get_Instance()->Get_FloatRandom(0.f, 360.f);
	
	_float4x4 Matrix;
	D3DXMatrixRotationY(&Matrix, D3DXToRadian(Angle));
	D3DXVec3TransformNormal(&m_vLook, &m_vLook, &Matrix);

	return S_OK;
}

void CItem::Tick(_float fTimeDelta)
{
	if (m_bDrop && !m_bDance)
	{
		m_fTimeAcc += fTimeDelta;
		if (0.01f < m_fTimeAcc)
		{
			_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			m_fDropY = vPos.y;
			m_bDance = true;
		}
	}


	MoveItem(fTimeDelta);
	GetItem(fTimeDelta);
}

void CItem::LateTick(_float fTimeDelta)
{

	if (!m_bDance)
	{
		m_pTransformCom->Go_Gravity(fTimeDelta);
		__super::BoxColCom_Tick(m_pTransformCom);
		m_pColliderCom->Add_PushBoxCollsionGroup(CCollider::COLLISION_ITEM, this);
	}

	
	m_pColliderCom->Add_BoxCollsionGroup(CCollider::COLLISION_ITEM, this);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CItem::Render()
{
	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_ItemInfo.iTextNum)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	// __super::BoxColCom_Render(m_pTransformCom);

	return S_OK;
}

void CItem::Collision(CGameObject * pOther)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (pOther->Get_Tag() == "Tag_Player")
	{
		if (pGameInstance->Key_Down(DIK_Z))
		{
			pGameInstance->PlaySoundW(L"PickUpItem.mp3", 24, 1.f);
			m_bGetItem = true;
			m_bDrop = false;
			m_bDance = false;

			m_pTransformCom->Set_Vel(5.f);
		}
	}

	if (pOther->Get_Tag() == "Tag_Cube" && !m_bGetItem)
	{
		m_bDrop = true;
	}


	Safe_Release(pGameInstance);
}

void CItem::Set_Billboard()
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

void CItem::MoveItem(_float fTimeDelta)
{
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (!m_bDrop && !m_bGetItem)
	{
		m_pTransformCom->Go_Dir(m_vLook, 1.5f, fTimeDelta);
	}
	else if(m_bDance)
	{
		if (m_fDropY >= vPos.y - 0.2f)
		{
			m_bUP = true;
		}
		else if(m_fDropY + 0.30f < vPos.y)
		{
			m_bUP = false;
		}

		if (m_bUP)
		{
			vPos.y += 0.1f* fTimeDelta;
		}
		else
		{
			vPos.y -= 0.1f* fTimeDelta;
		}

		 m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	 }
}

void CItem::GetItem(_float fTimeDelta)
{
	if (!m_bGetItem)
		return;

	m_fGetItemAcc += fTimeDelta;
	if (1.f < m_fGetItemAcc)
	{
		CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
		pInvenInstance->Check_Icon(m_ItemInfo.pTag, m_ItemInfo.eType, 1);
		Set_Dead();
	}

	CGameObject* pObj = CToolManager::Get_Instance()->GetPlayer();
	CTransform* pTran = (CTransform*)pObj->Get_ComponentPtr(TEXT("Com_Transform"));
	_float3 vPos = pTran->Get_State(CTransform::STATE_POSITION);
	_float3 vDir = vPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransformCom->Go_Dir(vDir, 2.f, fTimeDelta);

}

HRESULT CItem::SetUp_Components()
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

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CItem::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->CulRUByLook(m_vLookTemp);

	return S_OK;

}

CItem * CItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem*		pInstance = new CItem(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem*		pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}






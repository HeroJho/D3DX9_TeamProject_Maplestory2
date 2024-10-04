#include "stdafx.h"
#include "..\Public\Inventory.h"
#include "InvenManager.h"
#include "GameInstance.h"
#include "ConsumIcon.h"
#include "StuffIcon.h"


CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}


CInventory::CInventory(const CInventory& rhs)
	:CUI(rhs)
{
}

HRESULT CInventory::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CInventory::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_UIInfo, pArg, sizeof(UIINFO));

	__super::Initialize(pArg);
	m_bRender = false;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InvenFrame"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CInvenManager* pInvenInstance = CInvenManager::Get_Instance();
	pInvenInstance->Set_Inventory(this);

	return S_OK;
}

void CInventory::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Down(DIK_I))
	{
		if (m_bRender)
			CGameInstance::Get_Instance()->PlaySoundW(L"MenuDown.mp3", 25, 1.f);
		else
			CGameInstance::Get_Instance()->PlaySoundW(L"MenuUp.mp3", 25, 1.f);
		m_bRender = !m_bRender;
	}

	Safe_Release(pGameInstance);
}

void CInventory::LateTick(_float fTimeDelta)
{
	for (auto& iter : m_InvenImage)
	{
		iter->Set_UIMovePos(m_UIInfo);
	}

	for (size_t i = CInvenManager::TYPE_EQUIP; i < CInvenManager::TYPE_END - 1; ++i)
	{
		for (auto& iter : m_InvenIcon[i])
		{
			iter->Set_UIMovePos(m_UIInfo);
		}
	}
	if(m_bRender)
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CInventory::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	Set_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	return S_OK;
}

HRESULT CInventory::Add_InvenImage(CUI * pInvenImage)
{
	if (pInvenImage == nullptr)
		return E_FAIL;

	m_InvenImage.push_back(pInvenImage);
	Safe_AddRef(pInvenImage);

	return S_OK;
}

HRESULT CInventory::Add_InvenIcon(CInvenManager::InvenType eType, CUI* pInvenIcon)
{
	if (pInvenIcon == nullptr)
		return E_FAIL;

	m_InvenIcon[eType].push_back(pInvenIcon);
	Safe_AddRef(pInvenIcon);

	return S_OK;
}

HRESULT CInventory::Set_Icon(const _tchar * pTag, CInvenManager::InvenType eType, _uint iNum)
{
	for (auto iter : m_InvenIcon[eType])
	{	
		switch (eType)
		{
		case Client::CInvenManager::TYPE_EQUIP:

			break;
		case Client::CInvenManager::TYPE_CONSUM:
		{
			CConsumIcon* pConsumIcon = (CConsumIcon*)iter;
			if (pConsumIcon->Get_Tag() == pTag)
			{
				pConsumIcon->Set_NowNum(iNum);
				return S_OK;
			}
		}
		case Client::CInvenManager::TYPE_STUFF:
		{
			CStuffIcon* pStuffIcon = (CStuffIcon*)iter;
			if (pStuffIcon->Get_Tag() == pTag)
			{
				pStuffIcon->Set_NowNum(iNum);
				return S_OK;
			}
		}
		default:
			break;
		}				
	}

	for (auto iter : m_InvenIcon[eType])
	{
		switch (eType)
		{
		case Client::CInvenManager::TYPE_EQUIP:
			break;
		case Client::CInvenManager::TYPE_CONSUM:
		{
			CConsumIcon* pConsumIcon = (CConsumIcon*)iter;
			if (pConsumIcon->Get_Tag() == TEXT("DefaultInfo"))
			{
				pConsumIcon->Set_ItemInfo(pTag);
				pConsumIcon->Set_NowNum(iNum);
				return S_OK;
			}
			break;
		}
		case Client::CInvenManager::TYPE_STUFF:
		{
			CStuffIcon* pStuffIcon = (CStuffIcon*)iter;
			if (pStuffIcon->Get_Tag() == TEXT("DefaultInfo"))
			{
				pStuffIcon->Set_ItemInfo(pTag);
				pStuffIcon->Set_NowNum(iNum);
				return S_OK;
			}
			break;
		}
		default:
			break;
		}
	
	}

	return S_OK;
}

HRESULT CInventory::Change_Info(const _tchar* pTag, _uint iIndex, CInvenManager::InvenType eType)
{
	for (auto iter : m_InvenIcon[eType])
	{
		if (iter->Get_IndexNum() == iIndex)
		{
			/*if (eType = CInvenManager::TYPE_EQUIP)
			{
				((CConsumIcon*)iter)->Set_ItemInfo(pTag);
				return S_OK;
			}*/
			if (eType == CInvenManager::TYPE_CONSUM)
			{
				((CConsumIcon*)iter)->Set_ItemInfo(pTag);
				return S_OK;
			}
			else if (eType == CInvenManager::TYPE_STUFF)
			{
				((CStuffIcon*)iter)->Set_ItemInfo(pTag);
				return S_OK;
			}
		}
	}
	return S_OK;
}





CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory*		pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CInventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInventory::Clone(void * pArg)
{
	CInventory*		pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CInventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	for (auto& iter : m_InvenImage)
		Safe_Release(iter);
	m_InvenImage.clear();

	for (size_t i = CInvenManager::TYPE_EQUIP; i < CInvenManager::TYPE_END - 1; ++i)
	{
		for (auto& iter : m_InvenIcon[i])
		{
			Safe_Release(iter);
		}
		m_InvenIcon[i].clear();
	}
	__super::Free();
}


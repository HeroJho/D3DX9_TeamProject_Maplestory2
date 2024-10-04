#pragma once

#include "Client_Defines.h"
#include "UI.h"
#include "EquipBtn.h"
#include "ConsumBtn.h"
#include "StuffBtn.h"
#include "InvenManager.h"

BEGIN(Client)

class CInventory final : public CUI
{
public:
	CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Add_InvenImage(CUI* pInvenImage);
	HRESULT Add_InvenIcon(CInvenManager::InvenType eType, CUI* pInvenIcon);
	HRESULT Set_Icon(const _tchar* pTag, CInvenManager::InvenType eType, _uint iNum);
	HRESULT Change_Info(const _tchar* pTag, _uint iIndex, CInvenManager::InvenType eType);
	
public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


private:
	list<CUI*> m_InvenImage;
	list<CUI*> m_InvenIcon[CInvenManager::TYPE_END];
};

END


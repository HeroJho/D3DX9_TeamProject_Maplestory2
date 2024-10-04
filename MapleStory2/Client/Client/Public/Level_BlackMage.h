#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CLevel_BlackMage final : public CLevel
{
private:
	CLevel_BlackMage(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_BlackMage() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Npc(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Map(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Section(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

private:
	CCollider*				m_pColliderCom = nullptr;
	_float m_fCreateAcc;

public:
	static CLevel_BlackMage* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
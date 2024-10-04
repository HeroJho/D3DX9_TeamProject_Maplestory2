#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CLevel_BlackMageEnter final : public CLevel
{
private:
	CLevel_BlackMageEnter(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_BlackMageEnter() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Npc(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Map(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Section(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Spawner(const _tchar* pLayerTag);

private:
	HRESULT Ready_SkillFrameBtn(const _tchar* pLayerTag);
	HRESULT Ready_SkillIcon(const _tchar* pLayerTag);
	HRESULT Ready_SkillInfo();
	HRESULT Ready_QuickSlot(const _tchar* pLayerTag);
	HRESULT Ready_QuickSlotSkill(const _tchar* pLayerTag);

private:
	CCollider*				m_pColliderCom = nullptr;

public:
	static CLevel_BlackMageEnter* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END
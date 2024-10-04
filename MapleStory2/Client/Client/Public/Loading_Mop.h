#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLoading_Mop final : public CUI
{
public:
	enum MOPIMG {IMG_MUSHROOM, IMG_SLIME, IMG_END};

private:
	CLoading_Mop(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoading_Mop(const CLoading_Mop& rhs);
	virtual ~CLoading_Mop() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetImg(MOPIMG eImg) { m_eImg = eImg; }

private:
	HRESULT SetUp_Components();

	

private:
	MOPIMG m_eImg = IMG_SLIME;
	_float m_fTimeAcc = 0.f;
	_float m_fTime = 0.f;
	_float m_fCount = 0.f;
	_float m_fMaxCount = 0.f;

	_bool m_bOn = false;

	CTexture*				m_pSlimeTextureCom = nullptr;

public:
	static CLoading_Mop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


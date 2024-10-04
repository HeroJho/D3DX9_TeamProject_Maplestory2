#pragma once
#include "Client_Defines.h"

#include "UI.h"


BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)

class CTitle final : public CUI
{
private:
	CTitle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTitle(const CTitle& rhs);
	virtual ~CTitle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void PlayTitleCurMap(LEVEL eLevel);


private:
	HRESULT SetUp_Components();


private:
	LEVEL m_eCurLevel;
	_bool m_bStart = false;
	CAnimator*				m_pAnimatorCom = nullptr;
	_float m_fTimeAcc = 0.f;

public:
	static CTitle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END


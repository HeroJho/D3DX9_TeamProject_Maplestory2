#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator final : public CComponent
{
public:
	enum STATE {STATE_LOOF, STATE_ONCE, STATE_ONCEEND, STATE_END};

public:
	typedef struct tagAniInfo
	{
		const _tchar* pAnimationTag;
		const _tchar* pPreAnimationTag = nullptr;

		_float fDelay = 0.f;
		STATE eMode;

	}ANIINFO;

private:
	CAnimator();
	CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimator(const CAnimator& rhs);
	virtual ~CAnimator() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Create_Texture(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg);
	HRESULT Bind_Texture(_uint iCount);
	HRESULT Play_Ani(_float fTimeDelta);

public:
	HRESULT Play_Loof(_float fTimeDelta);
	HRESULT Play_Once(_float fTimeDelta);

public:
	ANIINFO Get_AniInfo() { return m_AniInfo; }
	_uint Get_AnimCount() { return m_iAnimCount; }

	void Set_AniInfo(const _tchar* pAnimationTag, _float fDelay, STATE eMode);

public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CAnimator* Clone(void* pAvg) override;
	virtual void Free() override;

public:
	class CTexture* Find_Component();
	
private:
	map<const _tchar*, class CTexture*> m_AniTexture;
	typedef map<const _tchar*, CTexture*> ANITEXTURE;

	_float m_fAnimPerTime;
	_uint m_iAnimCount;
	ANIINFO m_AniInfo;
};

END


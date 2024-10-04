#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UIManager.h"
BEGIN(Engine)
class CAnimator;
class CTexture;
class CRenderer;
class CCollider;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)

class CCreature : public CGameObject
{
public:
	enum STATE { STATE_IDLE, STATE_MOVE, STATE_JUMP, STATE_DJUMP, STATE_ATTACK, STATE_DASH, STATE_HIT, STATE_CHASE, STATE_DIE, STATE_CUTSCEEN, STATE_CUTSCEEN_2, STATE_KNOCKBACK, STATE_RESET, STATE_RETURN, STATE_END };
	enum DIR { DIR_L, DIR_R, DIR_U, DIR_D, DIR_LU, DIR_RU, DIR_LD, DIR_RD, DIR_END };

	typedef struct tagCreatureDesc
	{
		_float3 vPos;
		_float3 vScale;
		_float3 vRotationAix;
		_float  fAngle;
		_float  fColRad;
		_uint	iNum;
	}CRETUREDESC;

protected:
	CCreature(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCreature(const CCreature& rhs);
	virtual ~CCreature() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override {};
	virtual void OnLay(_float3 vOutDis) override;

public:
	virtual void SetState(STATE eState, DIR eDir);
	virtual void SetState(STATE eState);
	virtual void Set_Dead() override;
	void Set_JumpPower(_float fValue) { m_fJumpPower = fValue; }

public:
	virtual void SetAni();
	void Set_Billboard();

	virtual void Damaged(CGameObject* pOther) override {};
	void SetKnockBack(_float fPowerY, _float fPowerX, _float3 vDir);
	
protected:
	virtual void SetShadow(LEVEL eLevel, _float fScale);

protected:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CAnimator*				m_pAnimatorCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

protected:
	_float3 m_vLookTemp;
	STATE m_eCurState;
	DIR m_eDir;

	_float m_fJumpPower = 0.f;
	_float m_fOriJumpPower = 0.f;

	class CShadow* m_pShadow = nullptr;

	_float m_fKnockPowerY = 0.f;
	_float m_fKnockPowerX = 0.f;
	_float3 m_vKnockDir;

protected:
 	virtual HRESULT Set_RenderState();
	virtual HRESULT Reset_RenderState();

	void TickKnockBack(_float fTimeDelta);

protected:
	HRESULT SetUp_Components();

public:
	static CCreature* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
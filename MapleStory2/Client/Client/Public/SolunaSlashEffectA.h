#pragma once

#include "Creature.h"

BEGIN(Client)

class CSolunaSlashEffectA final : public CCreature
{
public:
	typedef struct tagSolunaEffectADESC
	{
		DIR eDir;
	}SOLUNAEFFECTADESC;

private:
	CSolunaSlashEffectA(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSolunaSlashEffectA(const CSolunaSlashEffectA& rhs);
	virtual ~CSolunaSlashEffectA() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void Collision(CGameObject* pOther) override;

protected:
	virtual HRESULT Set_RenderState() override;
	virtual HRESULT Reset_RenderState() override;


public:
	void SetState(STATE eState, DIR eDir);
	void SetDirection();
	void SetPosition(DIR eDir);
public:
	virtual void SetAni() override;


private:
	STATE m_eCurState;
	DIR m_eDir;
	_bool m_bRender;
	CTransform* m_pTarget;
	SOLUNAEFFECTADESC m_Desc;
private:
	HRESULT SetUp_Components();

public:
	static CSolunaSlashEffectA* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
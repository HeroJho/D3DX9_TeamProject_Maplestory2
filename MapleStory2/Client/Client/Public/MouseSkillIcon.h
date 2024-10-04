#pragma once
#include "UI.h"

BEGIN(Client)

class CMouseSkillIcon final : public  CUI
{
private:
	CMouseSkillIcon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMouseSkillIcon(const CMouseSkillIcon& rhs);
	virtual ~CMouseSkillIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();
	_uint Get_TextNum() { return m_iTexturenum; }

public:
	static CMouseSkillIcon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


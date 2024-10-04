#pragma once
#include "UI.h"
#include "SkillManager.h"
BEGIN(Client)
class CQuickSlotSkill final : public  CUI
{
private:
	CQuickSlotSkill(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuickSlotSkill(const CQuickSlotSkill& rhs);
	virtual ~CQuickSlotSkill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Texture();
	_uint Get_TextNum() { return m_iTexturenum; }
	_uint Get_IndexNum() { return m_iIndexNum; }
	const _tchar* Get_Tag() { return m_pSkillInfoTag; }
	const wchar_t* Get_Notice() { return m_pSkillNotice; }
	CSkillManager::SkillGrade Get_Grade() { return m_eGrade; }
	void Clear_Data();
	void Set_Data(_uint Textnum, const _tchar* Tag, CSkillManager::SkillGrade eGrade, const wchar_t* pNotice) { m_iTexturenum = Textnum, m_pSkillInfoTag = Tag, m_eGrade = eGrade, m_pSkillNotice = pNotice; }


private:
	_uint m_iIndexNum;
	const _tchar* m_pSkillInfoTag;
	CSkillManager::SkillGrade m_eGrade;
	const wchar_t* m_pSkillNotice;
	LPD3DXFONT m_NoticeFont;

	
public:
	static CQuickSlotSkill* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END


#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_DEFAULT, TYPE_CUBEMAP, TYPE_END };
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Bind_Texture(_uint iTextureIndex);
	_uint Get_Size() { return m_iNumTextures; }

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;
	LPDIRECT3DBASETEXTURE9 Get_Tex(_uint iTextNum) { return m_Textures[iTextNum]; }


private:
	/*LPDIRECT3DCUBETEXTURE9
	LPDIRECT3DTEXTURE9*/
	vector<LPDIRECT3DBASETEXTURE9>				m_Textures;
	typedef vector<LPDIRECT3DBASETEXTURE9>		TEXTURES;
	_uint m_iNumTextures;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
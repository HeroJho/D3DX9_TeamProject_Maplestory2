#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_ColorRect final : public CVIBuffer
{
private:
	CVIBuffer_ColorRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_ColorRect(const CVIBuffer_ColorRect& rhs);
	virtual ~CVIBuffer_ColorRect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _bool Picking(class CTransform* pTransform, _float3* pOut) override;

public:
	static CVIBuffer_ColorRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
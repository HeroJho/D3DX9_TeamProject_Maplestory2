#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Voxel final : public CVIBuffer
{
public:
	typedef struct tagVoxelDesc
	{
		_tchar cFileName[256] = {0};
		_float3 vPos;
		_float3 vScale;
		_float3 vRotationAix;
	}VOXELDESC;

private:
	CVIBuffer_Voxel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Voxel(const CVIBuffer_Voxel& rhs);
	virtual ~CVIBuffer_Voxel() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* Tag);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Voxel* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* Tag);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CRedPortionInfo final : public CItemInfo
{
public:
	CRedPortionInfo();
	virtual ~CRedPortionInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


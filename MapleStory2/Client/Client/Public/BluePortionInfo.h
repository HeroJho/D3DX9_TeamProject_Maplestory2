#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CBluePortionInfo final : public CItemInfo
{
public:
	CBluePortionInfo();
	virtual ~CBluePortionInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

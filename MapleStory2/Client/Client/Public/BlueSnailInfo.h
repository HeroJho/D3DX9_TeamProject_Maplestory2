#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CBlueSnailInfo final : public CItemInfo
{
public:
	CBlueSnailInfo();
	virtual ~CBlueSnailInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

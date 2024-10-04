#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CRedSnailInfo final : public CItemInfo
{
public:
	CRedSnailInfo();
	virtual ~CRedSnailInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

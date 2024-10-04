#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CSlimeInfo final : public CItemInfo
{
public:
	CSlimeInfo();
	virtual ~CSlimeInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CGreenMushroomInfo final : public CItemInfo
{
public:
	CGreenMushroomInfo();
	virtual ~CGreenMushroomInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

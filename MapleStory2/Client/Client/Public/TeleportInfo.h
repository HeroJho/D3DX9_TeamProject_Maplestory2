#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CTeleportInfo final : public CItemInfo
{
public:
	CTeleportInfo();
	virtual ~CTeleportInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

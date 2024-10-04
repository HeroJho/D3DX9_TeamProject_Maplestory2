#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CRibbonPigInfo final : public CItemInfo
{
public:
	CRibbonPigInfo();
	virtual ~CRibbonPigInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

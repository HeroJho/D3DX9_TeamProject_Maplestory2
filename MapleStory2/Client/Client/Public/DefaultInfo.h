#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CDefaultInfo final : public CItemInfo
{
public:
	CDefaultInfo();
	virtual ~CDefaultInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


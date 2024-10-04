#pragma once
#include "Client_Defines.h"
#include "ItemInfo.h"
BEGIN(Client)
class CGoStumpInfo final : public CItemInfo
{
public:
	CGoStumpInfo();
	virtual ~CGoStumpInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};

END

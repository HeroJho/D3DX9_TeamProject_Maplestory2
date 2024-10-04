#pragma once
#include "Client_Defines.h"
#include "Item.h"

BEGIN(Client)

class CConsumItem final : public CItem
{
public:
	CConsumItem();
	virtual ~CConsumItem() = default;
};

END


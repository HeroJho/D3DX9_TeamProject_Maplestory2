#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CClientGameInstance final : CBase
{
	DECLARE_SINGLETON(CClientGameInstance)
private:
	CClientGameInstance();
	virtual ~CClientGameInstance() = default;



public:
	virtual void Free() override;
};

END
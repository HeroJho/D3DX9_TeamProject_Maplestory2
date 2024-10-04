#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CBeastInfo final : public CSkillInfo
{
public:
	CBeastInfo();
	virtual ~CBeastInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


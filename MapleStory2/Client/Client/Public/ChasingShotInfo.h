#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CChasingShotInfo final : public CSkillInfo
{
public:
	CChasingShotInfo();
	virtual ~CChasingShotInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


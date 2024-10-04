#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CReefAttackInfo final : public CSkillInfo
{
public:
	CReefAttackInfo();
	virtual ~CReefAttackInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


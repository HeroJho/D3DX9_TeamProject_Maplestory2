#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CWarriorReefInfo final : public CSkillInfo
{
public:
	CWarriorReefInfo();
	virtual ~CWarriorReefInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


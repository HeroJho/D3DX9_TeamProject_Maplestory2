#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CSunCrossInfo final : public CSkillInfo
{
public:
	CSunCrossInfo();
	virtual ~CSunCrossInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


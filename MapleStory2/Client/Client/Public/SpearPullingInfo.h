#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CSpearPullingInfo final : public CSkillInfo
{
public:
	CSpearPullingInfo();
	virtual ~CSpearPullingInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CCardinalBlastInfo final : public CSkillInfo
{
public:
	CCardinalBlastInfo();
	virtual ~CCardinalBlastInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


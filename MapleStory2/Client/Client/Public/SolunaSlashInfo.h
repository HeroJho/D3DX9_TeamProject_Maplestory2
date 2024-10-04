#pragma once
#include "Client_Defines.h"
#include "SkillInfo.h"
BEGIN(Client)
class CSolunaSlashInfo final : public CSkillInfo
{
public:
	CSolunaSlashInfo();
	virtual ~CSolunaSlashInfo() = default;

	void Initialize();

public:
	virtual void Free() override;
};
END


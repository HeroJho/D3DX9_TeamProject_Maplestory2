#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Model_MiniBlock.h"

BEGIN(Engine)
class CCollider;
class CGameObject;
END


BEGIN(Client)

class CToolManager final : CBase
{
	DECLARE_SINGLETON(CToolManager)

private:
	CToolManager();
	virtual ~CToolManager() = default;


public:
	// For. LevelChange
	void SetDestLevel(LEVEL eLevel, _float3 vDestPos);	
	void CheckDestLevel(CCollider* pColliderCom, LPDIRECT3DDEVICE9 pGraphic_Device);
	LEVEL Get_CurLevel();

	// For. Player
	CGameObject* GetPlayer();


	// For. MiniGame
	void MakeMiniBlock(_float3 vPos, CModel_MiniBlock::MINICOLOR eType);
	void MakeMiniDownCube();

	CModel_MiniBlock::MINICOLOR Get_MiniPicked() { return m_ePickedMiniBlock; }
	void Set_MiniPicked(CModel_MiniBlock::MINICOLOR ePickedMiniBlock) { m_ePickedMiniBlock = ePickedMiniBlock; }

	_bool Get_MiniBingo() { return m_bMiniBingo; }
	void Set_MiniBingo(_bool bMiniBingo) { 
		m_ePickedMiniBlock = CModel_MiniBlock::MINI_END;
		m_bMiniBingo = bMiniBingo;
	}
	void Set_MiniEnd() { m_bEndMini = true; }
	_bool Get_MiniEnd() { return m_bEndMini; }
	void DecreaseMonCount() { --m_iMonsterCount; }
	_int GetMonCount() { return m_iMonsterCount; }


	// .For BlackMage
	void Set_BlackCubeOn(_bool bBlackCubeOn) { m_bBlackCubeOn = bBlackCubeOn; }
	_bool Get_BlackCubeOn() { return m_bBlackCubeOn; }

	void DecreaseBlackCubeCount() { 
		--m_iBlackCubeCount; 
		if (0 > m_iBlackCubeCount) 
			m_iBlackCubeCount = 0; 
	}
	void IncreaseBlackCubeCount() {
		++m_iBlackCubeCount;
	}
	void Set_BlackCubeCount() { m_iBlackCubeCount = 0; }
	_int Get_BlackCubeCount() { return m_iBlackCubeCount; }


private:
	LEVEL m_eDestLevel = LEVEL_END;
	_float3 m_vDestPos;
	CModel_MiniBlock::MINICOLOR m_ePickedMiniBlock = CModel_MiniBlock::MINI_END;
	_bool m_bMiniBingo = false;
	_bool m_bEndMini = false;
	_int m_iMonsterCount = 6;

	_bool m_bBlackCubeOn = false;
	_int m_iBlackCubeCount = 0;

public:
	virtual void Free() override;
};

END
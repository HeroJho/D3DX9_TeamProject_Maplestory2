#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "InvenManager.h"

BEGIN(Client)

class CQuestManager final : CBase
{
	DECLARE_SINGLETON(CQuestManager)

public:
	CQuestManager();
	virtual ~CQuestManager() = default;

public:
	void Tick();


public:	//퀘스트의 상태를 변화시킴
	void QuestPrepare() { m_iQuestState = QUEST_PREPARE; }
	void QuestProgress() { m_iQuestState = QUEST_PROGRESS; }
	void QuestClear() { m_iQuestState = QUEST_CLEAR; }
	void QuestEnd() { m_iQuestState = QUEST_END; }
	
	//퀘스트의 상태를 리턴
	_uint Set_QuestState() { return m_iQuestState; }

	// 퀘스트의 시작과 끝을 리턴, 설정함
	void Check_End_Quest() { m_bCheckQuest = false; }
	void Check_Start_Quest() { m_bCheckQuest = true; }
	bool Check_Quest() { return m_bCheckQuest; }

	// 몬스터가 죽을때 인자로 몬스터의 이름을 넣고 호출시킴, 퀘스트의 클리어를 담당
	void Hunting(const _tchar* MonsterName);

	// 아이템을 먹을때 인자로 아이템의 이름을 넣음, 아이템 퀘스트 클리어를 담당
	void Eat_Item(const _tchar* ItemName);


	// 사냥한 몬스터 마릿수 리턴
	_uint Set_Slime(){ return m_iHuntSlime; }
	_uint Set_OrangeMushroom() { return m_iHuntOrangeMushroom; }
	_uint Set_StoneGolem() { return m_iHuntStoneGolem; }
	_uint Set_GAS() { return m_iGAS; }
	_uint Set_BlackMage() { return m_iBlackMage; }

	// 먹은 아이템수 리턴
	_uint Set_RedShell() { return m_iRedShell; }
	_uint Set_BlueShell() { return m_iBlueShell; }
	_uint Set_PigRibbon() { return m_iPigRibbon; }
	_uint Set_SlimeEssence() { return m_iSlimeEssence; }
	_uint Set_StumpFirewood() { return m_iStumpFirewood; }
	_uint Set_GreenMushroom() { return m_iGreenMushroom; }

	void Get_RedShell(_int RedShell) { m_iRedShell = RedShell; }
	void Get_BlueShell(_int BlueShell) { m_iBlueShell = BlueShell; }
	void Get_PigRibbon(_int PigRibbon) { m_iPigRibbon = PigRibbon; }
	void Get_SlimeEssence(_int SlimeEssence) { m_iSlimeEssence = SlimeEssence; }
	void Get_StumpFirewood(_int StumpFirewood) { m_iStumpFirewood = StumpFirewood; }
	void Get_GreenMushroom(_int GreenMushroom) { m_iGreenMushroom = GreenMushroom; }

	// 사냥한 모든 몬스터 초기화
	void Reset_Hunt() { m_iHuntSlime = 0; m_iHuntOrangeMushroom = 0; m_iHuntStoneGolem = 0; }

	// 몇번째 퀘스트인지를 GETSET
	void Set_First() { m_iQuestNum = QUEST_FIRST; }
	void Set_Second() { m_iQuestNum = QUEST_SECOND; }
	void Set_Third() { m_iQuestNum = QUEST_THIRD; }
	void Set_Fourth() { m_iQuestNum = QUEST_FOURTH; }
	void Set_Fifth() { m_iQuestNum = QUEST_FIFTH; }
	void Set_Sixth() { m_iQuestNum = QUEST_SIXTH; }
	_uint Get_QuestNum() { return m_iQuestNum; }

public:
	virtual void Free() override;


public:
	enum QUEST_STATE { QUEST_PREPARE, QUEST_PROGRESS, QUEST_CLEAR, QUEST_END};
	enum QUEST_HUNT { HUNT_SLIME, HUNT_ORANGEMUSHROOM, HUNT_END};
	enum QUEST_NUM { QUEST_START, QUEST_FIRST, QUEST_SECOND, QUEST_THIRD , QUEST_FOURTH, QUEST_FIFTH, QUEST_SIXTH};

private:
	_uint	m_iQuestState;
	_uint	m_iHunt;
	bool	m_bCheckQuest;

	//사냥한 몬스터를 갖고있는 변수
	_uint	m_iHuntSlime;
	_uint	m_iHuntOrangeMushroom;
	_uint	m_iHuntRibbonPig;
	_uint	m_iHuntStoneGolem;
	_uint	m_iGAS;

	//획득한 아이템을 갖고있는 변수
	_uint	m_iRedShell;
	_uint	m_iBlueShell;
	_uint	m_iPigRibbon;

	_uint	m_iSlimeEssence;
	_uint	m_iStumpFirewood;
	_uint	m_iGreenMushroom;
	_uint	m_iBlackMage;

	// 몇번째 퀘스트인지
	_uint	m_iQuestNum;
};

END
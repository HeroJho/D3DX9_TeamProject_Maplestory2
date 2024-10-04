#pragma once


#include "Client_Defines.h"
#include "Camera_Free.h"


BEGIN(ENGINE)
class CGameObject;
END

BEGIN(Client)

class CCutSceneManager final : CBase
{
	DECLARE_SINGLETON(CCutSceneManager)

private:
	CCutSceneManager();
	virtual ~CCutSceneManager() = default;


public:
	void Tick(_float fTimeDelta);

public:
	class CCamera_Free* Get_MainCam();

	_bool IsCutScene() { return m_bIsCutScene; }

	void Start_Enter_Henesys_1(); 
	void End_Enter_Henesys_1();

	void Start_Enter_Henesys_2();
	void End_Enter_Henesys_2();

	void Start_Enter_Elenya();
	void End_Enter_Elenya();

	void Start_Enter_ElenyaEnter();
	void End_Enter_ElenyaEnter();

	void Start_Enter_GASHenesys();
	void End_Enter_GASHenesys();

	void Start_Enter_InitGASHenesys();
	void End_Enter_InitGASHenesys();

	void Start_Enter_InitDHenesys();
	void End_Enter_InitDHenesys();

	void Start_Enter_DHenesys();
	void End_Enter_DHenesys();

	void Start_Enter_BlackMage();
	void End_Enter_BlackMage();


	void SetCamPos(_float3 vPos);

public:
	void Enter_Henesys_1(_float fTimeDelta);
	void Enter_Henesys_2(_float fTimeDelta);
	void Enter_Elenya(_float fTimeDelta);
	void Enter_ElenyaEnter(_float fTimeDelta);

	void Enter_GASHenesys(_float fTimeDelta);
	void Set_Jang(CGameObject* pJang) { m_pJang = pJang; }
	CGameObject* Get_Jang() { return m_pJang; }
	void Set_JangRander(_bool JangRander) { m_bJangRender = JangRander;}
	_bool Get_jangRander() { return m_bJangRender; }

	void Enter_InitGASHenesys(_float fTimeDelta);

	void Enter_InitDHenesys(_float fTimeDelta);

	void Enter_DHenesys(_float fTimeDelta);

	void Enter_BlackMage(_float fTimeDelta);


private:
	//.for total
	_bool m_bIsCutScene = false;

	_bool m_bHenesys_1 = false;
	_bool m_bHenesys_2 = false;
	_bool m_bElenya = false;
	_bool m_bElenyaEnter = false;
	_bool m_bGASHenesys = false;
	_bool m_bInitGASHenesys = false;
	_bool m_bInitDHenesys = false;
	_bool m_bDHenesys = false;
	_bool m_bBlackMage = false;

	//.for Henesys_1
	_bool	m_bEnter_Henesys_1 = false;
	_float	m_fHenesys_1_TimeAcc = 0.f;
	_bool	m_b1 = false;

	//.for Henesys_2
	_bool	m_bEnter_Henesys_2 = false;
	_float	m_fHenesys_2_TimeAcc = 0.f;
	_bool	m_b2_1 = false;
	_bool	m_b2_2 = false;

	//.for Elenya
	_bool	m_bEnter_Elenya = false;
	_float	m_fElenya_TimeAcc = 0.f;
	_bool	m_b3_1 = false;

	// for ElenyaEnter
	_float m_bEnter_ElenyaEnter = false;
	_float m_fElenyaEnter_TimeAcc = 0.f;
	_bool m_b4_1 = false;

	//.for DHenesys
	_float m_bEnter_DHenesys = false;
	_float m_fDHenesys_TimeAcc = 0.f;
	_bool m_b5_1 = false;

	//.for GASHenesys
	_bool	m_bEnter_GASHenesys = false;
	_float	m_fGASHenesys_TimeAcc = 0.f;
	_bool	m_b6 = false;
	CGameObject* m_pJang = nullptr;
	_bool m_bJangRender = true;

	//.for EnterGASHenesys
	_float m_bEnter_InitGASHenesys = false;
	_float m_fInitGASHenesys_TimeAcc = 0.f;
	_bool m_b7 = false;
	_bool m_b7_1 = false;

	//.for InitDHenesys
	_float m_bEnter_InitDHenesys = false;
	_float m_fInitDHenesys_TimeAcc = 0.f;
	_bool m_b8 = false;
	_bool m_b8_1 = false;
	_bool m_b8_2 = false;

	//.for BlackMage
	_float m_bEnter_BlackMage = false;
	_float m_fBlackMage_TimeAcc = 0.f;
	_bool m_b9 = false;



	// .For Total
	_float m_fMainUITimeAcc = 0.f;
	_bool m_bMainUI = false;

public:
	virtual void Free() override;

};


END

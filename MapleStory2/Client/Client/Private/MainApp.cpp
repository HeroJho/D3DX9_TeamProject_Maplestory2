#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "QuestManager.h"
#include "SkillManager.h"
#include "MouseManager.h"
#include "QuickSlotManager.h"
#include "UIManager.h"
#include "InvenManager.h"
#include "SpawnerManager.h"
#include "ParticleManager.h"
#include "CutSceneManager.h"
#include "ToolManager.h"
#include "Loading.h"
#include "Loading_Mop.h"
#include "CutScreen.h"
#include "DomiScreen.h"
#include "Title.h"
#include "Shader.h"



bool g_bStaticClone;
bool g_bStatic;
bool g_bGamePlay;
bool g_bHenesys;
bool g_bElenyaEnter;
bool g_bElenya;
bool g_bElenyaGAS;
bool g_bDHenesys;
bool g_bDHenesysHunting;
bool g_bDElenyaEnter;
bool g_bDElenya;
bool g_bBlackMageEnter;
bool g_bBlackMage;
bool g_bWhiteMage;


using namespace Client;

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC			GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = GRAPHICDESC::MODE_WIN;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;


	g_bStaticClone = false;
	g_bStatic = false;
	g_bGamePlay = false;
	g_bHenesys = false;
	g_bElenya = false;
	g_bElenyaEnter = false;

	g_bElenyaGAS = false;
	g_bDHenesys = false;
	g_bDHenesysHunting = false;
	g_bDElenyaEnter = false;
	g_bDElenya = false;
	g_bBlackMageEnter = false;
	g_bBlackMage = false;
	g_bWhiteMage = false;
	

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Default_SamplerState()))
		return E_FAIL;

	if (FAILED(Ready_Default_RenderState()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Loading_ForLoading()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_STATIC)))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return ;

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif  _DEBUG

	m_pGameInstance->Tick_Engine(fTimeDelta);

	CCutSceneManager::Get_Instance()->Tick(fTimeDelta);
	CUIManager::Get_Instance()->Tick(fTimeDelta);
	CUIManager::Get_Instance()->LateTick(fTimeDelta);
	CQuestManager::Get_Instance()->Tick();

	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_PLAYER, CCollider::COLLSION_POTAL);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_PLAYER, CCollider::COLLSION_MONSTER);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_PLAYER_SKILL, CCollider::COLLSION_MONSTER);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_PLAYER, CCollider::COLLISION_ITEM);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_PLAYER, CCollider::COLLSION_MONSTER_SKILL);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_PLAYER_SKILL, CCollider::COLLSION_MONSTER_SKILL);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_MONSTER, CCollider::COLLSION_MONSTER_SKILL);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_MONSTER, CCollider::COLLSION_MONSTER_SKILL);
	m_pCollider->Check_BoxCollsion(CCollider::COLLSION_MONSTER_SKILL, CCollider::COLLSION_PUSHBLOCK);


	m_pCollider->Check_PushBoxCollsion(CCollider::COLLSION_PUSHBLOCK, CCollider::COLLSION_PLAYER);
	m_pCollider->Check_PushBoxCollsion(CCollider::COLLSION_MONSTER, CCollider::COLLSION_PUSHBLOCK);
	m_pCollider->Check_PushBoxCollsion(CCollider::COLLSION_PLAYER, CCollider::COLLSION_DOWNBLOCK);
	m_pCollider->Check_PushBoxCollsion(CCollider::COLLSION_MODEL, CCollider::COLLSION_DOWNBLOCK);
	m_pCollider->Check_PushBoxCollsion(CCollider::COLLSION_MODEL, CCollider::COLLSION_PLAYER);


	m_pCollider->Check_PushCubeCollsion(CCollider::COLLSION_PUSHBLOCK);
	m_pCollider->Check_PushCubeCollsion(CCollider::COLLSION_TRIGGERBLOCK);
	m_pCollider->Check_PushCubeCollsion(CCollider::COLLSION_PLAYER);
	m_pCollider->Check_PushCubeCollsion(CCollider::COLLSION_MONSTER);
	m_pCollider->Check_PushCubeCollsion(CCollider::COLLISION_ITEM);
	m_pCollider->Check_PushCubeCollsion(CCollider::COLLSION_MODEL);
	
	

	m_pCollider->Check_SphereCollsion(CCollider::COLLSION_UI, CCollider::COLLSION_PLAYER);


	m_pCollider->End_Collsion();

	CToolManager::Get_Instance()->CheckDestLevel(m_pCollider, m_pGraphic_Device);
}

HRESULT CMainApp::Render()
{

	if (nullptr == m_pGameInstance)
		return E_FAIL;


	m_pGameInstance->Render_Begin();

	m_pRenderer->Draw();

	m_pGameInstance->Render_Level();

	m_pGameInstance->Render_End();

#ifdef _DEBUG
	++m_iNumDraw;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("fps : %d"), m_iNumDraw);
		m_iNumDraw = 0;
		m_fTimeAcc = 0.f;
	}

	 SetWindowText(g_hWnd, m_szFPS);
#endif // _DEBUG

	

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* 특정 레벨을 할당하기 전에 로딩레벨을 거쳐 할당되는 형태로 가자. */

	/* 할당한 레벨을 레벨매니져에 보관할 수 있또록. gksek. */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Default_SamplerState()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CMainApp::Ready_Default_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Collider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider"),
		m_pCollider = CCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_BoxCollider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BoxCollider"),
		CBoxCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Animator"),
		CAnimator::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Player"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Player.hlsl")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Cube.hlsl")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{

	return S_OK;
}


HRESULT CMainApp::Loading_ForLoading()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OrangeMushroom_Move"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/OrangeMushroom/OrangeMushroom_Move%d.png"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Slime_Move"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Slime/Slime_Move%d.png"), 7))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/Loading/Loading_%d.png"), 16))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CutScreen"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/CutScreen/CutScreen_%d.png"), 11))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DomiScreen"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/DomiScreen/DomiScreen_%d.png"), 33))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Henesys"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/MapTile/Tile_Henesys/MapTile_%d.png"), 22))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_HenesysHunting"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/MapTile/Tile_HenesysHunting/MapTile_%d.png"), 22))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_ElenyaEnter"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/MapTile/Tile_ElenyaEnter/MapTile_%d.png"), 22))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Elenya"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/MapTile/Tile_Elenya/MapTile_%d.png"), 22))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_EndOfWorl"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/MapTile/Tile_EndOfWorl/MapTile_%d.png"), 22))))
		return E_FAIL;





	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Loading"),
		CLoading::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Loading_Mop"),
		CLoading_Mop::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CutScreen"),
		CCutScreen::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DomiScreen"),
		CDomiScreen::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Title"),
		CTitle::Create(m_pGraphic_Device))))
		return E_FAIL;

	CUIManager::Get_Instance()->Set_Loading();

	Safe_Release(pGameInstance);

	return S_OK;
}






CMainApp * CMainApp::Create()
{	
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CMainApp"));
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CMainApp::Free()
{
	CToolManager::Destroy_Instance();
	CCutSceneManager::Destroy_Instance();
	CParticleManager::Destroy_Instance();
	CQuestManager::Destroy_Instance();
	CMouseManager::Destroy_Instance();
	CQuickSlotManager::Destroy_Instance();
	CSkillManager::Destroy_Instance();
	CUIManager::Destroy_Instance();
	CInvenManager::Destroy_Instance();
	CSpawnerManager::Destroy_Instance();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();

}

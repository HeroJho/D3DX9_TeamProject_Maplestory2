#include "stdafx.h"
#include "..\Public\Chat.h"
#include "GameInstance.h"
#include "QuestManager.h"
#include "SkillManager.h"
#include "InvenManager.h"
#include "ItemInfo.h"
#include "CutSceneManager.h"
#include "ToolManager.h"

CChat::CChat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_UIInfo, sizeof(UIINFO));
}

CChat::CChat(const CChat & rhs)
	: CGameObject(rhs)
{
}

HRESULT CChat::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChat::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_bChat = false;
	m_bCountCheck = false;
	m_bCountCheck2 = false;
	m_bWhy = false;
	m_iRandom = 0;

	m_fCount = 0;
	m_fCountChat = 0;

	m_iChatProgress = 0;

	D3DXCreateFont(m_pGraphic_Device, 45, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Consolas"), &m_pFontName);

	D3DXCreateFont(m_pGraphic_Device, 33, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		TEXT("Arial"), &m_pFont);

	D3DXMatrixOrthoLH(&m_ProjMatrix, _float(g_iWinSizeX), _float(g_iWinSizeY), 0, 1);

	m_UIInfo.fSizeX = 1024.0f;
	m_UIInfo.fSizeY = 512.0f;
	m_UIInfo.fX = 650.0f;
	m_UIInfo.fY = 500.0f;

	m_pTransformCom->Set_Scaled(_float3(m_UIInfo.fSizeX, m_UIInfo.fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_UIInfo.fX - g_iWinSizeX * 0.5f, -m_UIInfo.fY + g_iWinSizeY * 0.5f, 0.f));


	return S_OK;
}

void CChat::Tick(_float fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	RECT		rcUI;
	SetRect(&rcUI, _int(m_UIInfo.fX - m_UIInfo.fSizeX * 0.5f), _int(m_UIInfo.fY - m_UIInfo.fSizeY * 0.5f), _int(m_UIInfo.fX + m_UIInfo.fSizeX * 0.5f), _int(m_UIInfo.fY + m_UIInfo.fSizeY * 0.5f));

	CQuestManager* pInstance = CQuestManager::Get_Instance();

	if (m_bCountCheck)
		m_fCount += 1.f * fTimeDelta;

	// npc채팅이 켜졌을때 채팅의 카운트를 증가시킴
	if(m_bCountCheck2)
		m_fCountChat += 1.f * fTimeDelta;
}

void CChat::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	CQuestManager* pInstance = CQuestManager::Get_Instance();


}

HRESULT CChat::Render()
{
	RECT rc;
	_float4x4		Matrix;
	D3DXMatrixIdentity(&Matrix);



	//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Matrix);
	m_pTransformCom->Bind_WorldMatrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &Matrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	CQuestManager* pInstance = CQuestManager::Get_Instance();

	m_bChat = pInstance->Check_Quest(); 



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_ComponentPtr(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), 0);

	_float3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (((vPlayerPos.x > 7.f && vPlayerPos.x < 15.f) && ((vPlayerPos.z) < 0.f && vPlayerPos.z > -3.f)) || ((CToolManager::Get_Instance()->Get_CurLevel() == LEVEL_WHITEMAGE) && (vPlayerPos.x > 0.f && vPlayerPos.x < 5.f)))
	{

		if (pInstance->Set_QuestState() == 1)
		{
			if (CGameInstance::Get_Instance()->Key_Down(DIKEYBOARD_SPACE))
			{
				m_bWhy = true;
			}
			if (m_bWhy)
			{
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];
					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));


					if (m_iRandom == 0)
					{
						wsprintf(m_cNPCChat, TEXT("자네 아직도 안갔나?"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iRandom == 1)
					{
						wsprintf(m_cNPCChat, TEXT("허허..나 젊었을때는 이정도 의뢰는 10분이면.."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}


					else if (m_iRandom == 2)
					{
						wsprintf(m_cNPCChat, TEXT("흠..."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}


					if (CGameInstance::Get_Instance()->Key_Down(DIKEYBOARD_RETURN))
					{
						m_bWhy = false;
						m_fCount = 0;
						m_iRandom = (CGameInstance::Get_Instance()->Get_Random(0, 2));
					}
				}

			}
		}


		if (pInstance->Get_QuestNum() == 1)
		{
			// 퀘스트매니져를 통해 가져온 채팅값이 TRUE이면 채팅을 그림, 엔터를 누르면 퀘스트 전구를 진행으로 바꾸고 채팅값을 FALSE로 만듦
			if (m_bChat && pInstance->Set_QuestState() == 0)
			{
				Bind_Animation();
				m_pVIBufferCom->Render();

				// 시작 애니메이션이 다 그려지면
				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];

					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					if (CGameInstance::Get_Instance()->Key_Down(DIKEYBOARD_SPACE))
						m_iChatProgress++;

					if (m_iChatProgress == 0)
					{
						wsprintf(m_cNPCChat, TEXT("오오..기다리고 있었네, 몸은 좀 어떤가 용사여."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 1)
					{
						wsprintf(m_cNPCChat, TEXT("자네를 찾고 있었는데 금방 와주었구만."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 2)
					{
						wsprintf(m_cNPCChat, TEXT("사실 마을에 큰 문제가 생겼네."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 3)
					{
						wsprintf(m_cNPCChat, TEXT("자네도 알다시피 우리 헤네시스 주민들은\n농장을 운영하여 생활하고 있네"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 4)
					{
						wsprintf(m_cNPCChat, TEXT("헤네시스에 이런저런 몬스터가 많긴하지만 그동안 달팽이들이 가끔\n농작물을 몇개 훔쳐먹는걸 제외하고는 딱히 농장에 큰 피해는 없었지."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 5)
					{
						wsprintf(m_cNPCChat, TEXT("그런데 주황버섯 녀석들이 농장에 먹을것이 있다는것을 알아차렸는지,\n지난주부터 단체로 나타나서는 농작물을 전부 훔쳐먹고 망치는게 아닌가!"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 6)
					{
						wsprintf(m_cNPCChat, TEXT("이대로 가다간 모두 굶어 죽고 말거야"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else
					{
						wsprintf(m_cNPCChat, TEXT("그러니 시간만 괜찮다면 자네가 주황버섯들을 혼내줬으면 하는데..\n가능하겠는가?"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}
				}

				if ((GetKeyState(VK_RETURN) & 0x8000))
				{
					pInstance->QuestProgress();
					pInstance->Check_End_Quest();
					m_bCountCheck = false;
					m_fCount = 0;
					CGameInstance::Get_Instance()->PlaySound(L"QuestStart.wav", 13, 1.f);
				}
			}

			if (m_bChat && pInstance->Set_QuestState() == 2)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];
					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					wsprintf(m_cNPCChat, TEXT("벌써 다 잡았는가?\n고맙네!"));
					SetRect(&rc, 350, 530, 0, 0);
					m_pFont->DrawText(NULL, m_cNPCChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}

				if (GetKeyState(VK_RETURN) & 0x8000)
				{
					m_fCount = 0;
					pInstance->QuestPrepare();
					pInstance->Check_End_Quest();
					pInstance->Reset_Hunt();
					CQuestManager::Get_Instance()->Set_Second();
					CSkillManager::Get_Instance()->Set_SkillPoint(3);
					m_iChatProgress = 10;
					CGameInstance::Get_Instance()->PlaySound(L"QuestClear.wav", 13, 1.f);
				}
			}
		}

		else if (pInstance->Get_QuestNum() == 2)
		{
			// 퀘스트매니져를 통해 가져온 채팅값이 TRUE이면 채팅을 그림, 엔터를 누르면 퀘스트 전구를 진행으로 바꾸고 채팅값을 FALSE로 만듦
			if (m_bChat && pInstance->Set_QuestState() == 0)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];

					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					if (CGameInstance::Get_Instance()->Key_Down(DIKEYBOARD_SPACE))
						m_iChatProgress++;

					if (m_iChatProgress == 10)
					{
						wsprintf(m_cNPCChat, TEXT("아, 자네구만.. 주황버섯을 잡아준건 정말 고마웠네,\n마을에 도움이 많이 됐어."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 11)
					{
						wsprintf(m_cNPCChat, TEXT("...그런데 혹시 자네 시간 좀 있나?\n개인적으로 부탁하고 싶은게 있어서.."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 12)
					{
						wsprintf(m_cNPCChat, TEXT("사실 다음주가 내 아내인 밍밍부인 생일인데,\n선물을 좀 주고 싶어서 말이야"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 13)
					{
						wsprintf(m_cNPCChat, TEXT("내 이런걸 해본적 없어서\n젊은 자네가 더 잘 알지 않겠나"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else
					{
						wsprintf(m_cNPCChat, TEXT("혹시 선물로 줄만한 걸 찾아다 줄 수 있겠나?"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}
				}
				if ((GetKeyState(VK_RETURN) & 0x8000))
				{
					pInstance->QuestProgress();
					pInstance->Check_End_Quest();
					m_bCountCheck = false;
					m_fCount = 0;
					CGameInstance::Get_Instance()->PlaySound(L"QuestStart.wav", 13, 1.f);
				}
			}

			if (m_bChat && pInstance->Set_QuestState() == 2)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];
					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					wsprintf(m_cNPCChat, TEXT("...요새 젊은 사람들 취향은 이런가?\n크흠 아무튼 고맙네!"));
					SetRect(&rc, 350, 530, 0, 0);
					m_pFont->DrawText(NULL, m_cNPCChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}

				if (GetKeyState(VK_RETURN) & 0x8000)
				{
					m_fCount = 0;
					m_bCountCheck = false;
					pInstance->QuestPrepare();
					pInstance->Check_End_Quest();
					pInstance->Reset_Hunt();
					CQuestManager::Get_Instance()->Set_Third();
					CSkillManager::Get_Instance()->Set_SkillPoint(3);
					m_iChatProgress = 20;

					CItemInfo* RedSnailInfo = CInvenManager::Get_Instance()->Get_ItemInfo(L"RedSnailInfo", CInvenManager::TYPE_STUFF);
					CItemInfo* BlueSnailInfo = CInvenManager::Get_Instance()->Get_ItemInfo(L"BlueSnailInfo", CInvenManager::TYPE_STUFF);
					CItemInfo* RibbonPigInfo = CInvenManager::Get_Instance()->Get_ItemInfo(L"RibbonPigInfo", CInvenManager::TYPE_STUFF);
					RedSnailInfo->Set_NowNum(0);
					BlueSnailInfo->Set_NowNum(0);
					RibbonPigInfo->Set_NowNum(0);
					CGameInstance::Get_Instance()->PlaySound(L"QuestClear.wav", 13, 1.f);
				}
			}
		}
		else if (pInstance->Get_QuestNum() == 3)
		{
			// 퀘스트매니져를 통해 가져온 채팅값이 TRUE이면 채팅을 그림, 엔터를 누르면 퀘스트 전구를 진행으로 바꾸고 채팅값을 FALSE로 만듦
			if (m_bChat && pInstance->Set_QuestState() == 0)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];

					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					if (CGameInstance::Get_Instance()->Key_Down(DIKEYBOARD_SPACE))
						m_iChatProgress++;

					if (m_iChatProgress == 20)
					{
						wsprintf(m_cNPCChat, TEXT("오, 용사여 오랜만이네,\n지난번 선물은 정말 고마웠네. 부인이 참 좋아하더군."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 21)
					{
						wsprintf(m_cNPCChat, TEXT("내가 사실 자네 뭐라도 주려고\n옆마을 엘리니아에 다녀오려고 했었네."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 22)
					{
						wsprintf(m_cNPCChat, TEXT("자네를 생각하며 기쁜 마음으로 나가려는데...이게 뭐람!\n엘리니아 가는 하나밖에 없는 길을 스톤 골렘이 막고있는게 아닌가!"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 23)
					{
						wsprintf(m_cNPCChat, TEXT("그래서 어쩔 수 없이 그냥 돌아왔다네...이렇게 난감한 일이 없지.\n누가 골렘을 잡아줄 사람이 없을까?\n 마을 사람들이 잡기엔 크게 다칠것이 분명한데..."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else
					{
						wsprintf(m_cNPCChat, TEXT("...자네가 잡아주겠다고? 이렇게 고마운 일이!\n역시 용사야! 믿고 있겠네!"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}
				}
				if ((GetKeyState(VK_RETURN) & 0x8000))
				{
					m_fCount = 0;
					pInstance->QuestProgress();
					pInstance->Check_End_Quest();
					CGameInstance::Get_Instance()->PlaySound(L"QuestStart.wav", 13, 1.f);
				}
			}

			if (m_bChat && pInstance->Set_QuestState() == 2)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];
					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					wsprintf(m_cNPCChat, TEXT("아니 이렇게 빨리!자네는 역시 헤네시스의 보물일세!\n정말 고맙네!"));
					SetRect(&rc, 350, 530, 0, 0);
					m_pFont->DrawText(NULL, m_cNPCChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}
				if (GetKeyState(VK_RETURN) & 0x8000)
				{
					{
						m_fCount = 0;
						m_bCountCheck = false;
						pInstance->QuestPrepare();
						pInstance->Check_End_Quest();
						pInstance->Reset_Hunt();
						CQuestManager::Get_Instance()->Set_Fourth();
						CSkillManager::Get_Instance()->Set_SkillPoint(3);
						m_iChatProgress = 30;
						CGameInstance::Get_Instance()->PlaySound(L"QuestClear.wav", 13, 1.f);
					}
				}

			}
		}


		else if (pInstance->Get_QuestNum() == 4)
		{
			// 퀘스트매니져를 통해 가져온 채팅값이 TRUE이면 채팅을 그림, 엔터를 누르면 퀘스트 전구를 진행으로 바꾸고 채팅값을 FALSE로 만듦
			if (m_bChat && pInstance->Set_QuestState() == 0)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];

					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					if (CGameInstance::Get_Instance()->Key_Down(DIKEYBOARD_SPACE))
						m_iChatProgress++;

					if (m_iChatProgress == 30)
					{
						wsprintf(m_cNPCChat, TEXT("저기.....저기! 저기 잠깐 멈춰보게!\n자네 왜 요즘 말을 걸어도 잘 듣지를 못하나?\n귀가 잘 안들린다고....? 이거 몸보신이라도 해야겠구만"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 31)
					{
						wsprintf(m_cNPCChat, TEXT("그래서 하는 말인데 내 자네에게 부탁을 자주 했지 않은가.\n이번에는 보양식 한그릇을 만들어주고 싶어서 말일세. 기다려보게."));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else if (m_iChatProgress == 32)
					{
						wsprintf(m_cNPCChat, TEXT("흠..이걸 어디 뒀더라....어라, 재료가 조금 모자라군! 이걸 어쩌나!\n....혹시 자네 지금 바쁜가?"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}

					else
					{
						wsprintf(m_cNPCChat, TEXT("바쁘지 않다면 부족한 재료를 조금 구해다 줄 수 있겠나?\n겸사겸사 내 몫도 해서 2인분만....맛은 보장하겠네!"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					}
				}
				if ((GetKeyState(VK_RETURN) & 0x8000))
				{
					m_fCount = 0;
					pInstance->QuestProgress();
					pInstance->Check_End_Quest();
					CGameInstance::Get_Instance()->PlaySound(L"QuestStart.wav", 13, 1.f);
				}
			}

			if (m_bChat && pInstance->Set_QuestState() == 2)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];
					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					wsprintf(m_cNPCChat, TEXT("재료를 다 모아왔구만! 내 금방 만들어주겠네!\n다 만들었군!내가 먼저 한입 먹어보고...\n우..우웩! 으으..."));
					SetRect(&rc, 350, 530, 0, 0);
					m_pFont->DrawText(NULL, m_cNPCChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}
				if (GetKeyState(VK_RETURN) & 0x8000)
				{
					m_fCount = 0;
					pInstance->QuestPrepare();
					pInstance->Check_End_Quest();
					pInstance->Reset_Hunt();
					CQuestManager::Get_Instance()->Set_Fifth();
					CSkillManager::Get_Instance()->Set_SkillPoint(3);

					CItemInfo* SlimeInfo = CInvenManager::Get_Instance()->Get_ItemInfo(L"SlimeInfo", CInvenManager::TYPE_STUFF);
					CItemInfo* GoStumpInfo = CInvenManager::Get_Instance()->Get_ItemInfo(L"GoStumpInfo", CInvenManager::TYPE_STUFF);
					CItemInfo* GreenMushroomInfo = CInvenManager::Get_Instance()->Get_ItemInfo(L"GreenMushroomInfo", CInvenManager::TYPE_STUFF);

					SlimeInfo->Set_NowNum(0);
					GoStumpInfo->Set_NowNum(0);
					GreenMushroomInfo->Set_NowNum(0);
					CCutSceneManager::Get_Instance()->Start_Enter_GASHenesys();
					CGameInstance::Get_Instance()->PlaySound(L"QuestClear.wav", 13, 1.f);
				}
			}
		}



		else if (pInstance->Get_QuestNum() == 5)
		{
			// 퀘스트매니져를 통해 가져온 채팅값이 TRUE이면 채팅을 그림, 엔터를 누르면 퀘스트 전구를 진행으로 바꾸고 채팅값을 FALSE로 만듦
			if (m_bChat && pInstance->Set_QuestState() == 0)
			{
				m_bCountCheck = true;
				{
				if (m_fCount <= 0.1)
					m_pTextureCom->Bind_Texture(0);
				else if (0.1 < m_fCount && m_fCount <= 0.12)
					m_pTextureCom->Bind_Texture(1);
				else if (0.12 < m_fCount && m_fCount <= 0.14)
					m_pTextureCom->Bind_Texture(2);
				else if (0.14 < m_fCount && m_fCount <= 0.16)
					m_pTextureCom->Bind_Texture(3);
				else if (0.16 < m_fCount && m_fCount <= 0.18)
					m_pTextureCom->Bind_Texture(4);
				else if (0.18 < m_fCount && m_fCount <= 0.2)
					m_pTextureCom->Bind_Texture(5);
				else if (0.2 < m_fCount && m_fCount <= 0.22)
					m_pTextureCom->Bind_Texture(6);
				else if (0.22 < m_fCount && m_fCount <= 0.24)
					m_pTextureCom->Bind_Texture(7);
				else if (0.24 < m_fCount && m_fCount <= 0.26)
					m_pTextureCom->Bind_Texture(8);
				else if (0.26 < m_fCount && m_fCount <= 0.28)
					m_pTextureCom->Bind_Texture(9);
				else if (0.28 < m_fCount && m_fCount <= 0.3)
					m_pTextureCom->Bind_Texture(10);
				else if (0.3 < m_fCount && m_fCount <= 0.32)
					m_pTextureCom->Bind_Texture(11);
				else if (0.32 < m_fCount && m_fCount <= 0.34)
					m_pTextureCom->Bind_Texture(12);
				else if (0.34 < m_fCount && m_fCount <= 0.36)
					m_pTextureCom->Bind_Texture(13);
				else if (0.36 < m_fCount && m_fCount <= 0.38)
					m_pTextureCom->Bind_Texture(14);
				else if (0.38 < m_fCount && m_fCount <= 0.4)
					m_pTextureCom->Bind_Texture(15);
				else if (0.4 < m_fCount && m_fCount <= 0.42)
					m_pTextureCom->Bind_Texture(16);
				else
				{
					m_pTextureCom->Bind_Texture(17);
					m_bCountCheck = false;
				}
				}
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];

					wsprintf(cChat, TEXT("나"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

						wsprintf(m_cNPCChat, TEXT("(... 장로스탄이 납치된 것 같다. 구해주러 가자.)"));
						SetRect(&rc, 350, 530, 0, 0);
						m_pFont->DrawText(NULL, m_cNPCChat,
							-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
					

				}
				if ((GetKeyState(VK_RETURN) & 0x8000))
				{
					m_fCount = 0;
					pInstance->QuestProgress();
					pInstance->Check_End_Quest();
					CGameInstance::Get_Instance()->PlaySound(L"QuestStart.wav", 13, 1.f);
				}
			}

			if (m_bChat && pInstance->Set_QuestState() == 2)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];
					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					wsprintf(m_cNPCChat, TEXT("구해줘서 정말 고맙네!"));
					SetRect(&rc, 350, 530, 0, 0);
					m_pFont->DrawText(NULL, m_cNPCChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}
				if (GetKeyState(VK_RETURN) & 0x8000)
				{
					m_fCount = 0;
					pInstance->QuestPrepare();
					pInstance->Check_End_Quest();
					pInstance->Reset_Hunt();
					CSkillManager::Get_Instance()->Set_SkillPoint(3);
					CQuestManager::Get_Instance()->Set_Sixth();

					CCutSceneManager::Get_Instance()->Start_Enter_InitDHenesys();
				}
			}
		}

		else if (pInstance->Get_QuestNum() == 6)
		{
			// 퀘스트매니져를 통해 가져온 채팅값이 TRUE이면 채팅을 그림, 엔터를 누르면 퀘스트 전구를 진행으로 바꾸고 채팅값을 FALSE로 만듦
			if (m_bChat && pInstance->Set_QuestState() == 0)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];

					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					if (CGameInstance::Get_Instance()->Key_Down(DIKEYBOARD_SPACE))
						m_iChatProgress++;

					wsprintf(m_cNPCChat, TEXT("어째서 이런일이...\n 마을을 파괴한 자는 엘리니아 쪽으로 도망갔어.\n 부디 우리 마을을 구해주게.."));
					SetRect(&rc, 350, 530, 0, 0);
					m_pFont->DrawText(NULL, m_cNPCChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

				}
				if ((GetKeyState(VK_RETURN) & 0x8000))
				{
					m_fCount = 0;
					pInstance->QuestProgress();
					pInstance->Check_End_Quest();
				}
			}

			if (m_bChat && pInstance->Set_QuestState() == 2)
			{
				m_bCountCheck = true;
				Bind_Animation();
				m_pVIBufferCom->Render();

				if (!m_bCountCheck)
				{
					m_bCountCheck2 = true;
					TCHAR cChat[128];
					_tchar m_cNPCChat[128];
					wsprintf(cChat, TEXT("장로스탄"));
					SetRect(&rc, 350, 500, 0, 0);
					m_pFont->DrawText(NULL, cChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 0.45f, 0.f, 1.f));

					wsprintf(m_cNPCChat, TEXT("마을을 지켜줘서 고맙네 허허허!!! \n 어?..."));
					SetRect(&rc, 350, 530, 0, 0);
					m_pFont->DrawText(NULL, m_cNPCChat,
						-1, &rc, DT_NOCLIP, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
				}
				if (GetKeyState(VK_RETURN) & 0x8000)
				{
					m_fCount = 0;
					pInstance->QuestClear();
					pInstance->Check_End_Quest();
					pInstance->Reset_Hunt();
					CSkillManager::Get_Instance()->Set_SkillPoint(3);
					CQuestManager::Get_Instance()->Set_Sixth();
				}
			}
		}
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CChat::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

void CChat::MouseCollision()
{
}

CChat * CChat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChat*		pInstance = new CChat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CChat"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChat::Clone(void* pArg)
{
	CChat*		pInstance = new CChat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Created : CChat clone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChat::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

void CChat::Bind_Animation()
{
	m_bCountCheck = true;
	if (m_fCount <= 0.1)
		m_pTextureCom->Bind_Texture(0);
	else if (0.1 < m_fCount && m_fCount <= 0.12)
		m_pTextureCom->Bind_Texture(1);
	else if (0.12 < m_fCount && m_fCount <= 0.14)
		m_pTextureCom->Bind_Texture(2);
	else if (0.14 < m_fCount && m_fCount <= 0.16)
		m_pTextureCom->Bind_Texture(3);
	else if (0.16 < m_fCount && m_fCount <= 0.18)
		m_pTextureCom->Bind_Texture(4);
	else if (0.18 < m_fCount && m_fCount <= 0.2)
		m_pTextureCom->Bind_Texture(5);
	else if (0.2 < m_fCount && m_fCount <= 0.22)
		m_pTextureCom->Bind_Texture(6);
	else if (0.22 < m_fCount && m_fCount <= 0.24)
		m_pTextureCom->Bind_Texture(7);
	else if (0.24 < m_fCount && m_fCount <= 0.26)
		m_pTextureCom->Bind_Texture(8);
	else if (0.26 < m_fCount && m_fCount <= 0.28)
		m_pTextureCom->Bind_Texture(9);
	else if (0.28 < m_fCount && m_fCount <= 0.3)
		m_pTextureCom->Bind_Texture(10);
	else if (0.3 < m_fCount && m_fCount <= 0.32)
		m_pTextureCom->Bind_Texture(11);
	else if (0.32 < m_fCount && m_fCount <= 0.34)
		m_pTextureCom->Bind_Texture(12);
	else if (0.34 < m_fCount && m_fCount <= 0.36)
		m_pTextureCom->Bind_Texture(13);
	else if (0.36 < m_fCount && m_fCount <= 0.38)
		m_pTextureCom->Bind_Texture(14);
	else if (0.38 < m_fCount && m_fCount <= 0.4)
		m_pTextureCom->Bind_Texture(15);
	else if (0.4 < m_fCount && m_fCount <= 0.42)
		m_pTextureCom->Bind_Texture(16);
	else if (0.42 < m_fCount && m_fCount <= 0.44)
		m_pTextureCom->Bind_Texture(17);
	else if (0.44 < m_fCount && m_fCount <= 0.47)
		m_pTextureCom->Bind_Texture(18);
	else if (0.47 < m_fCount && m_fCount <= 0.5)
		m_pTextureCom->Bind_Texture(19);
	else if (0.5 < m_fCount && m_fCount <= 0.53)
		m_pTextureCom->Bind_Texture(20);
	else if (0.53 < m_fCount && m_fCount <= 0.56)
		m_pTextureCom->Bind_Texture(21);
	else if (0.56 < m_fCount && m_fCount <= 0.59)
		m_pTextureCom->Bind_Texture(22);
	else
	{
		m_pTextureCom->Bind_Texture(23);
		m_bCountCheck = false;
	}
}

void CChat::Create_Chat(string Chat)
{
}


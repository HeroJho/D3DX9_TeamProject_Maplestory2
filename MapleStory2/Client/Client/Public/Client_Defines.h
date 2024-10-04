#pragma once

namespace Client
{
#pragma warning (disable : 4244)
#pragma warning (disable : 4305)
#pragma warning (disable : 4309)
	static const unsigned int g_iWinSizeX = 1280;
	static const unsigned int g_iWinSizeY = 720;
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_HENESYS, LEVEL_ELENYAENTER, LEVEL_ELENYA, LEVEL_GAS ,LEVEL_DHENESYS, LEVEL_DHENESYSHUNTING, LEVEL_DELENYAENTER, LEVEL_DELENYA, LEVEL_DARKMAGEJUMP, LEVEL_DARKMAGEENTER, LEVEL_DARKMAGE, LEVEL_WHITEMAGE, LEVEL_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern bool g_bStaticClone;
extern bool g_bStatic;
extern bool g_bGamePlay;
extern bool g_bHenesys;
extern bool g_bElenya;
extern bool g_bElenyaEnter;

extern bool g_bElenyaGAS;
extern bool g_bDHenesys;
extern bool g_bDHenesysHunting;
extern bool g_bDElenyaEnter;
extern bool g_bDElenya;
extern bool g_bBlackMageEnter;
extern bool g_bBlackMage;
extern bool g_bWhiteMage;

using namespace Client;

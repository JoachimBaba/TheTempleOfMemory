#pragma once

#include "singleton.h"

struct TGfxSprite;
struct TGfxTexture;
class TInterface;
class TScreenOverlay;

enum EMenuPage
{
	EMenuPage_Main,
	EMenuPage_Instructions,
	EMenuPage_Trophies,

	EMenuPage_Count,
	EMenuPage_Null
};

enum EMainMenuState
{
	EMainMenuState_Play,
	EMainMenuState_Instructions,
	EMainMenuState_Trophies,
	EMainMenuState_Count,
	EMainMenuState_Null
};

class TMenu : public TSingleton<TMenu>
{
	friend class TSingleton<TMenu>;
public:	
	bool Update();
	virtual void Render();

private:
	TMenu();
	virtual ~TMenu();

	static TGfxTexture * s_pTexture[EMenuPage_Count];
	TInterface * m_pPage[EMenuPage_Count];
	TGfxSprite * m_pBackground[EMenuPage_Count];
	EMenuPage m_eActualPage;
	EMenuPage m_eNextPage;
	TScreenOverlay * m_pScreenOverlay;
};

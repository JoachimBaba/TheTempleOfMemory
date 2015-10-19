#pragma once

#include "singleton.h"

enum EControlerState
{
	EControlerState_Menu,
	EControlerState_Game
};

class TGame;
class TMenu;
class TSetting;
class TTrophy;
class TSoundSystem;

class TControler : public TSingleton<TControler>
{
	friend class TSingleton<TControler>;
public:
	void Update();
	void Render();

private:
	TControler();
	~TControler();

	TMenu * m_pMenu;
	TGame * m_pGame;
	TSetting * m_pSetting;
	TTrophy * m_pTrophy;
	TSoundSystem * m_pSounds;
	
	EControlerState m_eState;
};

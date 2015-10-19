#include "flib.h"
#include "flib_vec2.h"

#include "texture_controler.h"
#include "game.h"
#include "menu.h"
#include "setting.h"
#include "trophy.h"
#include "sound.h"

#include "controler.h"

TControler::TControler()
	:m_pMenu( nullptr ), m_pGame( nullptr ), m_pSetting( nullptr ), m_pTrophy( nullptr ), m_pSounds( nullptr ),
	m_eState( EControlerState_Menu )
{
	TTextureControler::CreateInstance();
	m_pSetting = TSetting::CreateInstance();
	m_pTrophy = TTrophy::CreateInstance();
	m_pSounds = TSoundSystem::CreateInstance();

	switch( m_eState )
	{
	case EControlerState_Menu:
		m_pMenu = TMenu::CreateInstance();
		break;
	case EControlerState_Game:
		m_pGame = TGame::CreateInstance( START_ROOM_COUNT, EGame_State_Playing, *m_pSetting );
		break;
	default:
		break;
	}
}

TControler::~TControler()
{
	m_pSounds->DestroyInstance();
	m_pTrophy->DestroyInstance();
	m_pSetting->DestroyInstance();
	m_pMenu->DestroyInstance();
	m_pGame->DestroyInstance();
	TTextureControler::DestroyInstance();
}

void TControler::Update()
{
	switch (m_eState)
	{
	case EControlerState_Menu:
	{
		if( m_pGame != nullptr )
		{
			TGame::DestroyInstance();
			m_pGame = nullptr;
		}

		bool bMenuResult = m_pMenu->Update();

		if( bMenuResult == false )
		{
			m_eState = EControlerState_Game;
			GfxDbgPrintf("menu->game\n");

			m_pGame = TGame::CreateInstance( START_ROOM_COUNT, EGame_State_Playing, *m_pSetting );
		}
		//else
		//	GfxDbgPrintf( "menu->menu\n" );//dbg
	}
		break;
	case EControlerState_Game:
	{
		bool bGameContinue = m_pGame->Update();
		
		if( bGameContinue == false )
		{
			m_eState = EControlerState_Menu;
			GfxDbgPrintf( "game->menu\n" );
		}
	}
		break;
	default:
		break;
	}
}

void TControler::Render()
{
	switch (m_eState)
	{
	case EControlerState_Menu:
		if( m_pMenu != nullptr )
			m_pMenu->Render();
		break;
	case EControlerState_Game:
		if(m_pGame != nullptr)
			m_pGame->Render();
		break;
	}
}


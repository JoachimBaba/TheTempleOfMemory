#include "flib.h"
#include "flib_vec2.h"
#include "lua.h"

#include "interface.h"
#include "square.h"

#include "trophy.h"

#include "sound.h"
#include "constants.h"

lua_State * TTrophy::s_pLuaState = nullptr;
TSquare * TTrophy::s_pTextInfo = nullptr;
TSquare * TTrophy::s_pMessage = nullptr;
TInterface * TTrophy::s_pTrophies = nullptr;
TGfxSprite * TTrophy::s_pScore = nullptr;

int LuaUnlockTrophy( lua_State * L );
int LuaIncreaseScore( lua_State * L );

TTrophy::TTrophy()
{
	//trophy display :::::::::::::::::::::::::::::::::::::::::::
	s_pTrophies = new TInterface(TrophySettup);
	s_pMessage = new TSquare(TGfxVec2(SCREEN_SIZE_X/2.f, SCREEN_SIZE_Y - 50.f));

	//text overlay ::::::::::::::::::::::::::::::::::::::::::::::
	s_pTextInfo = nullptr;

	//lua settup ::::::::::::::::::::::::::::::::::::::::::::::::
	s_pLuaState = luaL_newstate();
	luaL_openlibs( s_pLuaState );
	
	//push functions on the pile
	lua_pushcfunction( s_pLuaState, LuaUnlockTrophy );
	lua_setglobal( s_pLuaState, "LuaUnlockTrophy" );

	lua_pushcfunction( s_pLuaState, LuaIncreaseScore );
	lua_setglobal( s_pLuaState, "LuaIncreaseScore" );
	
	//init score
	s_pScore = GfxTextSpriteCreate();
	GfxSpriteSetFilteringEnabled( s_pScore, false );
	
	//init lua program
	LuaDoFile( s_pLuaState, "score.lua" );
}

TTrophy::~TTrophy()
{
	delete s_pTrophies;
}

void TTrophy::Update()
{
	int iTrophySelected = s_pTrophies->CheckInput();

	if( iTrophySelected < BUTTON_MAX )
	{
		lua_pushinteger( s_pLuaState, iTrophySelected+1 );
		LuaCallGlobal( s_pLuaState, "LuaSendMessage" );

		const char * pText = luaL_checkstring( s_pLuaState, -1 );
		s_pMessage->ActivateText( pText, TEXT_INFO_DURATION );
		lua_pop( s_pLuaState, -1 );
	}

	s_pMessage->UpdateText();
}

void TTrophy::Render()
{
	s_pTrophies->Render();
	GfxTextSpriteRender( s_pScore, 50.f, 20.f, EGfxColor_White, 2.f, false, false );
	s_pMessage->RenderText();
}

void TTrophy::UpdateScore()
{
	LuaCallGlobal( s_pLuaState, "LuaUpdateScore" );
}

void TTrophy::ResetScores( EScore eScore )
{
	lua_pushinteger( s_pLuaState, int( eScore )+1 );
	LuaCallGlobal( s_pLuaState, "LuaResetScores" );
}

void TTrophy::UnlockTrophy( int iTrophy, const char * pMessage )
{
//	GfxDbgPrintf( "Trophy : %d, %s\n", iTrophy, pMessage );//dbg
	if( iTrophy <= TROPHY_MAX )
	{
		//display achevement feedback
		if( s_pTextInfo != nullptr )
			s_pTextInfo->ActivateText( pMessage, TEXT_INFO_DURATION * 4 );

		//show trophy in trophy screen
		int i = iTrophy - 1;

		s_pTrophies->UpdateButton( i, EButtonAnimState_Available);

		TSoundSystem::Play( ESound_Trophy );
	}
}

int LuaUnlockTrophy( lua_State * L )
{
	int i = (int) luaL_checknumber( L, -2 );
	const char * pText = luaL_checkstring( L, -1 );

	//GfxDbgPrintf( "Trophy : %d, %s\n", i, pText );

	TTrophy::UnlockTrophy( i, pText );

	return 0;
}

int LuaIncreaseScore( lua_State * L )
{
	const char * pText = luaL_checkstring( L, -1 );
	GfxTextSpritePrintf( TTrophy::GetScore(), pText );

	return 0;
}


#pragma once

#include "singleton.h"

struct lua_State;
class TSquare;
struct TGfxVec2;
class TInterface;

const int TROPHY_MAX = 10;

enum EScore
{
	EScore_NotItem,
	EScore_NoDamage,
	EScore_NoWrongDoor,
	EScore_All
};

class TTrophy : public TSingleton<TTrophy>
{
	friend class TSingleton<TTrophy>;
public:
	static void Update();
	static void UpdateScore();
	static void ResetScores( EScore eScore );
	static void UnlockTrophy( int iTrophy, const char * pMessage );
	static void Render();
	static void SetTextOverlay( TSquare & pTextOverlay ) { s_pTextInfo = &pTextOverlay; };
	static TGfxSprite * GetScore() { return s_pScore; };
private:
	TTrophy();
	virtual ~TTrophy();
	static lua_State * s_pLuaState;
	static TSquare * s_pTextInfo;
	static TSquare * s_pMessage;
	static TGfxSprite * s_pScore;
	static TInterface * s_pTrophies;
};


#pragma once

#include "game.h"
#include "room.h"

struct TGfxVec2;
struct TWait;

enum ETransitionState
{
	ETransitionState_DoorOpening,
	ETransitionState_RoomTransit,
	ETransitionState_DoorClosing
};

class TTransition
{
public:
	TTransition( ERoomResult eTransitionType, TRoom & pCurrentRoom, TRoom & pOtherRoom, int iDelay );
	~TTransition();

	EGame_State Update();
	void Render();

private:
	TGfxVec2 m_tDir;
	ERoomResult m_eTransitionType;
	TRoom * m_pCurrentRoom;//room from where we come
	TRoom * m_pOtherRoom;//room to where we go
	TWait * m_pTimer;
	ETransitionState m_eState;
};

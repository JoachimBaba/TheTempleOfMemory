#include "flib.h"
#include "flib_vec2.h"

#include "room.h"
#include "constants.h"

#include "door.h"

TDoor::TDoor(const TGfxVec2 & tPos, float fOrient, TGfxVec2 tRadius, TDoor * pOtherSide, TRoom * pRoom)
	: TSquare( tPos, fOrient, tRadius, ETextureData_Door ),
	m_pOtherSide(pOtherSide),
	m_pRoom(pRoom),
	m_eState(EDoor_State_Open)
{
}

TDoor::~TDoor()
{
	m_pOtherSide = nullptr;
	m_pRoom = nullptr;
}

void TDoor::Update()
{
	
}

void TDoor::SetDoorState( const EDoor_State eNewState )
{
	if( eNewState != m_eState )
	{
		m_eState = eNewState;
		SetTile( 0, eNewState );
	}
};

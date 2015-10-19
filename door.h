#pragma once

#include "square.h"
struct TGfxVec2;
class TRoom;

enum EDoor_State
{
	EDoor_State_Open,
	EDoor_State_Locked,
	EDoor_State_Shown,

	EDoor_State_Count
};

class TDoor : public TSquare
{
public:
	TDoor(const TGfxVec2 & tPos, float fOrient, TGfxVec2 tRadius, TDoor * pOtherSide, TRoom * pRoom);
	virtual ~TDoor();

	virtual void Update();

	void SetOtherSideDoor( const TDoor & pOtherSide ) { m_pOtherSide = &pOtherSide; };
	const TDoor & GetOtherSideDoor()const { return *m_pOtherSide; };
	TRoom & GetRoom()const { return *m_pRoom; };
	EDoor_State GetDoorState()const { return m_eState; };
	void SetDoorState( const EDoor_State eLockState );

private:
	const TDoor * m_pOtherSide;
	TRoom * m_pRoom;
	EDoor_State m_eState;
};

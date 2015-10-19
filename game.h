#pragma once

#include "singleton.h"

struct TGfxSprite;
struct TGfxTexture;
class TTransition;
class TScreenOverlay;

class TElement;
class TCircle;
class TPlayer;
class TDoor;
class TRoom;
class TItem;
class TSentinel;
class TSetting;
class TSquare;

template <class T>
class TSLL_Root;

enum EGame_State
{
	EGame_State_Playing,
	EGame_State_RoomTransition,
	EGame_State_Pause,
	EGame_State_Death,
	EGame_State_ToMenu
};

class TGame : public TSingleton<TGame>
{
	friend class TSingleton < TGame > ;
public:
	static TGame * CreateInstance( int iStartRooms, EGame_State eState, TSetting & pSetting );

	bool Update();
	void Render();
	
private:
	TSLL_Root<TRoom> * m_pRoomList;
	TPlayer * m_pPlayer;
	EGame_State m_eState;
	int m_iRoomCount;
	int m_iScore;
	TTransition * m_pTransitionParam;
	TScreenOverlay * m_pScreenOverlay;
	TSetting * m_pSetting;
	TSquare * m_pMenuButton;
	TSquare * m_pProgressInfo;

	TGame(int iStartRooms, EGame_State eState, TSetting & pSetting);
	~TGame();
	void AddRoom();
	void GenerateContent();
	void BoundDoors( TDoor & pDoor1, TDoor & pDoor2 );
};

#pragma once

#include "parser\sll_item.h"
#include "constants.h"
#include "game.h"

struct TGfxSprite;
struct TGfxTexture;
class TDoor;
class TItem;
class TElement;
class TDynamic;
class TPlayer;
class TSetting;

enum ESide
{
	ESide_Up,
	ESide_Down,
	ESide_Left,
	ESide_Right,

	ESide_Count,
	ESide_Null
};

enum ERoomResult
{
	ERoomResult_Playing,
	ERoomResult_WrongDoor,
	ERoomResult_ExitDoor,
	ERoomResult_EntryDoor,
	ERoomResult_VoidExitDoor,
	ERoomResult_Death
};

class TRoom : public TSLL_Item<TRoom>
{
public:
	TRoom(ESide eEntrySide, ESide eExitSide, int iId);
	virtual ~TRoom();

	//Content Init
	void DoorInit();
	void ObstacleInit();
	void EnnemyInit();
	void ItemInit();

	//Update actions method
	ERoomResult GameplayUpdate( TPlayer & pPlayer );
	void TransitionUpdate( const TGfxVec2 & tBackgroundPos );
	ERoomResult DynamicObjectUpdate( TPlayer & pPlayer );
	ERoomResult StaticObjectUpdate( TPlayer & pPlayer );
	ERoomResult HunterUpdate( TPlayer & pPlayer );
	ERoomResult DoorsUpdate( TPlayer & pPlayer );
	void ItemsUpdate( TPlayer & pPlayer );
	void ObstacleUpdate( TPlayer & pPlayer );

	void Render( EGame_State eGameState);

	//room content
	TDoor * GetDoor(int i)
	{
		GfxDbgAssert(i < ESide_Count, "array overflow, index is too great !");
		return m_pDoor[i];
	}
		
	const TDoor & GetEntryDoor()const { return *m_pDoor[m_eEntrySide]; };
	const TDoor & GetExitDoor()const { return *m_pDoor[m_eExitSide]; };

	TDoor & GetEntryDoor() { return *m_pDoor[m_eEntrySide]; };
	TDoor & GetExitDoor() { return *m_pDoor[m_eExitSide]; };

	ESide GetEntrySide()const { return m_eEntrySide; };
	ESide GetExitSide()const { return m_eExitSide; };

	void ShowExit();

	//room info
	int GetId()const { return m_iId; };
	TRoom * FindRoom( int iId );

	const TGfxVec2 & GetPos()const { return m_tPos; };
	TGfxSprite * GetSprite() { return m_pSprite; };
	void SetPos( const TGfxVec2 & tPos ) { m_tPos = tPos; GfxSpriteSetPosition( m_pSprite, m_tPos.x, m_tPos.y ); };
	
	static void InitTextures();
	static void DestroyTextures();
	static void InitSettings( TSetting * pSettings ) { s_pSetting = pSettings; };

private:
	TElement * m_pObstacles[OBSTACLE_MAX];
	TDynamic * m_pEnnemies[ENNEMY_MAX];
	TDoor * m_pDoor[ESide_Count];
	TItem * m_pItem[ITEM_MAX];
	static TGfxTexture * s_pBackgroundTexture[BG_COUNT];
	static TSetting * s_pSetting;
	TGfxVec2 m_tPos;
	ESide m_eEntrySide;
	ESide m_eExitSide;
	int m_iId;
	TGfxSprite * m_pSprite;
		
	void SetDoors( ESide eEntrySide, TGfxSprite * pSprite );
	void ResetEnnemiesPos();
};

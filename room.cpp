#include "flib.h"
#include "flib_vec2.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "element.h"
#include "square.h"
#include "door.h"
#include "circle.h"
#include "dynamic.h"
#include "player.h"
#include "item.h"
#include "sentinel.h"
#include "hunter.h"
#include "setting.h"

#include "room.h"

#include "constants.h"
#include "trophy.h"
#include "sound.h"

TGfxTexture * TRoom::s_pBackgroundTexture[BG_COUNT];
TSetting * TRoom::s_pSetting;

TRoom::TRoom(ESide eEntrySide, ESide eExitSide, int iId)
	: TSLL_Item(), m_tPos( TGfxVec2( fBACKGROUND_POS_X, fBACKGROUND_POS_Y ) ), m_eEntrySide( eEntrySide ), m_eExitSide( eExitSide ), m_iId( iId )
{
	//sprite
	int iTexture = 0;

	if( m_iId == 1 )
		iTexture = 0;
	else if(m_iId % 10 > 5 )
		iTexture = 1;
	else
		iTexture = 2;
	
	m_pSprite = GfxSpriteCreate( s_pBackgroundTexture[iTexture] );
	//doors
	DoorInit();

	//Items
	ItemInit();

	//Ennemies
	EnnemyInit();
	
	//Obstacles
	ObstacleInit();
}

TRoom::~TRoom()
{
	for (int i = 0; i < ESide_Count; i++)
	{
		delete m_pDoor[i];
	}
	for (int i = 0; i < ITEM_MAX; i++)
	{
		delete m_pItem[i];
	}
	for (int i = 0; i < ENNEMY_MAX; i++)
	{
		delete m_pEnnemies[i];
	}
	for (int i = 0; i < OBSTACLE_MAX; i++)
	{
		delete m_pObstacles[i];
	}

	GfxSpriteDestroy( m_pSprite );
}

void TRoom::DoorInit()
{
	TGfxVec2 tDoorSize = TGfxVec2( fDOOR_RADIUS_X, fDOOR_RADIUS_Y );

	for( int i = 0; i < ESide_Count; i++ )
	{
		TGfxVec2 tPos;

		switch( i )
		{
		case ESide_Up:
			tPos = TGfxVec2( SCREEN_SIZE_X / 2.f, fWALL_MARGIN_Y + fWALL_RADIUS_Y + fDOOR_RADIUS_Y + fDOOR_MARGIN_Y );
			m_pDoor[i] = new TDoor( tPos, 0.f, tDoorSize, nullptr, this );
			break;
		case ESide_Down:
			tPos = TGfxVec2( SCREEN_SIZE_X / 2.f, SCREEN_SIZE_Y - ( fWALL_MARGIN_Y + fWALL_RADIUS_Y + fDOOR_RADIUS_Y + fDOOR_MARGIN_Y ) );
			m_pDoor[i] = new TDoor( tPos, 180.f, tDoorSize, nullptr, this );
			break;
		case ESide_Left:
			tPos = TGfxVec2( fWALL_MARGIN_X + fWALL_RADIUS_Y + fDOOR_RADIUS_X + fDOOR_MARGIN_X, SCREEN_SIZE_Y / 2.f );
			m_pDoor[i] = new TDoor( tPos, 90.f, tDoorSize, nullptr, this );
			break;
		case ESide_Right:
			tPos = TGfxVec2( SCREEN_SIZE_X - ( fWALL_MARGIN_X + fWALL_RADIUS_Y + fDOOR_RADIUS_X + fDOOR_MARGIN_X ), SCREEN_SIZE_Y / 2.f );
			m_pDoor[i] = new TDoor( tPos, -90.f, tDoorSize, nullptr, this );
			break;
		}
	}
}

void TRoom::ObstacleInit()
{
	TGfxVec2 tHorWallSize = TGfxVec2( fWALL_RADIUS_X_HOR, fWALL_RADIUS_Y );
	TGfxVec2 tVertWallSize = TGfxVec2( fWALL_RADIUS_X_VERT, fWALL_RADIUS_Y );

	for( int i = 0; i < OBSTACLE_MAX; i++ )
	{
		if( i < 4 )
		{
			TGfxVec2 tPos;
			switch( i )
			{
			case ESide_Up:
				tPos = TGfxVec2( SCREEN_SIZE_X / 2.f, SCREEN_SIZE_Y - ( fWALL_MARGIN_Y + fWALL_RADIUS_Y ) );
				m_pObstacles[i] = new TSquare( tPos, 0.f, tHorWallSize );
				break;
			case ESide_Down:
				tPos = TGfxVec2( SCREEN_SIZE_X / 2.f, fWALL_MARGIN_Y + fWALL_RADIUS_Y );
				m_pObstacles[i] = new TSquare( tPos, 180.f, tHorWallSize );
				break;
			case ESide_Left:
				tPos = TGfxVec2( fWALL_MARGIN_X + fWALL_RADIUS_Y, SCREEN_SIZE_Y / 2.f );
				m_pObstacles[i] = new TSquare( tPos, -90.f, tVertWallSize );
				break;
			case ESide_Right:
				tPos = TGfxVec2( SCREEN_SIZE_X - ( fWALL_MARGIN_X + fWALL_RADIUS_Y ), SCREEN_SIZE_Y / 2.f );
				m_pObstacles[i] = new TSquare( tPos, 90.f, tVertWallSize );
				break;
			}
		}
		else
		{
			m_pObstacles[i] = nullptr;
		}
	}
}

void TRoom::EnnemyInit()
{
	int iEnnemyCount = 0;
	int iPatternCount = 0;
	int iPattern = 0;
	int eEnnemyType = ( EEnnemyType ) GfxMathGetRandomInteger( EEnnemyType_None, EEnnemyType_Count - 1 );

	if( s_pSetting != nullptr && eEnnemyType == EEnnemyType_Sentinel )//setting for ennemis that follows path
	{
		iPatternCount = s_pSetting->GetPatternsCount();
		iPatternCount--;
		if( iPatternCount < 0 )
			iPattern = 0;
		iPattern = GfxMathGetRandomInteger( 0, iPatternCount );
		iEnnemyCount = s_pSetting->GetEnnemyCount( iPattern );
	}
	else if( eEnnemyType == EEnnemyType_Wanderer )
	{
		iEnnemyCount = GfxMathGetRandomInteger( 2, 4 );
	}
	else
	{
		iEnnemyCount = 1;
	}

	for( int i = 0; i < ENNEMY_MAX; i++ )
	{
		if( m_iId % 2 == 0 &&//room pair
			m_iId > 1 )//not first room
		{
			if( i < iEnnemyCount )
			{
				switch( eEnnemyType )
				{
				case EEnnemyType_None:
					m_pEnnemies[i] = nullptr;
					break;
				case EEnnemyType_Sentinel:
				{
					TGfxVec2 * pDests = s_pSetting->GetPattern( iPattern, i );
					int iDestCount = s_pSetting->GetDestinationsCount( iPattern, i );
					m_pEnnemies[i] = new TSentinel( TGfxVec2( 0.f, 0.f ), 0.f, fHUNTER_RADIUS, pDests, iDestCount, 0 );
				}
					break;
				case EEnnemyType_Chaser:
					m_pEnnemies[i] = new THunter( i, fHUNTER_RADIUS, fFOLLOWER_SPEED, EHunterType_Chaser );
					break;
				case EEnnemyType_Wanderer:
					m_pEnnemies[i] = new THunter( i, fHUNTER_RADIUS, fFOLLOWER_SPEED, EHunterType_Wanderer );
					break;
				case EEnnemyType_Count:
					m_pEnnemies[i] = nullptr;
					break;
				default:
					m_pEnnemies[i] = nullptr;
					break;
				}
			}
			else
			{
				m_pEnnemies[i] = nullptr;
			}
		}
		else
		{
			m_pEnnemies[i] = nullptr;
		}
	}
}

void TRoom::ItemInit()
{
	TGfxVec2 tScreenCenter = TGfxVec2( SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y / 2.0f );

	int iKeyExist = GfxMathGetRandomInteger( 0, KEY_HAZARD );//on chance out of X (KEY_HAZARD to have a key)

	for( int i = 0; i < ITEM_MAX; i++ )
	{
		if( m_iId % 2 == 0 && m_iId > 1 )//items spawn every 2 rooms and not at first room
		{
			int iItemExist = GfxMathGetRandomInteger( 0, ITEM_CHANCE_MAX );//there is one chance out of ITEM_CHANCE_MAX to have an item
			if( iItemExist == 0 )
			{
				//Item position (range around the center of the screen)
				float fPosX = GfxMathGetRandomFloat( tScreenCenter.x - fITEMS_RANGE, tScreenCenter.x + fITEMS_RANGE );
				float fPosY = GfxMathGetRandomFloat( tScreenCenter.y - fITEMS_RANGE, tScreenCenter.y + fITEMS_RANGE );
				TGfxVec2 tPos = TGfxVec2( fPosX, fPosY );

				//Item type
				EItemType eType = EItemType_Key;

				if( iKeyExist == KEY_HAZARD )
				{
					iKeyExist = 0;
				}
				else if( iKeyExist < KEY_HAZARD )
				{
					eType = ( EItemType ) GfxMathGetRandomInteger( EItemType_Sword, EItemType_Locker );
				}

				//Item creation
				m_pItem[i] = new TItem( tPos, fITEMRADIUS, eType );
			}
			else
			{
				m_pItem[i] = nullptr;
			}
		}
		else
		{
			m_pItem[i] = nullptr;
		}
	}
}

ERoomResult TRoom::GameplayUpdate( TPlayer & pPlayer )
{
	ERoomResult eResult = ERoomResult_Playing;

	eResult = DynamicObjectUpdate( pPlayer );

	if( eResult == ERoomResult_Death )
		return eResult;

	eResult = StaticObjectUpdate( pPlayer );
	return eResult;
}

void TRoom::TransitionUpdate(const TGfxVec2 & tBackgroundPos)
{
	SetPos(tBackgroundPos);
	for (int i = 0; i < ESide_Count; ++i)
	{
		TGfxVec2 tTemporaryDoorPos = m_pDoor[i]->GetPos() + tBackgroundPos;
		GfxSpriteSetPosition(m_pDoor[i]->GetSprite(), tTemporaryDoorPos.x, tTemporaryDoorPos.y);
	}
}

void TRoom::Render( EGame_State eGameState )
{
	GfxSpriteRender( m_pSprite );
	
	for( int i = 0; i < ESide_Count; i++ )
	{
		if( m_pDoor[i] != nullptr )
			m_pDoor[i]->Render();
	}

	if( eGameState != EGame_State_RoomTransition )
	{
		for( int i = 0; i < ITEM_MAX; i++ )
		{
			if( m_pItem[i] != nullptr )
				m_pItem[i]->Render();
		}
		for( int i = 0; i < ENNEMY_MAX; i++ )
		{
			if( m_pEnnemies[i] != nullptr )
				m_pEnnemies[i]->Render();
		}

		for( int i = 0; i < OBSTACLE_MAX; i++ )
		{
			if( m_pObstacles[i] != nullptr )
				m_pObstacles[i]->Render();
		}
	}
}

ERoomResult TRoom::DynamicObjectUpdate( TPlayer & pPlayer )
{
	return HunterUpdate(pPlayer);
}

ERoomResult TRoom::HunterUpdate( TPlayer & pPlayer )
{
	for (int i = 0; i < ENNEMY_MAX; i++)
	{
		if (m_pEnnemies[i] != nullptr)
		{
			m_pEnnemies[i]->Update();

			//if player isn't healing or dying (on delay)
			if( !pPlayer.IsWaiting() && m_pEnnemies[i]->GetState() == EDynamicState_Active )
			{
				//collision test
				TGfxVec2 tCollideResult = m_pEnnemies[i]->Collide( pPlayer );

				if( tCollideResult.x != 0.f || tCollideResult.y != 0.f )
				{
					//player has protection
					if( pPlayer.CheckSelectedAndCollected( EItemType_Sword ) )
					{
						//sword stabs the ennemy
						pPlayer.YieldActualItem();
						m_pEnnemies[i]->Repulse( tCollideResult );

						//hurt's the ennemies
						m_pEnnemies[i]->SetState( EDynamicState_Hurt );
						m_pEnnemies[i]->SetAnimLevel( EDynamicAnimState_Death );
						m_pEnnemies[i]->SetAnimPlayOnce( true );

						TSoundSystem::Play( ESound_Sword );
					}
					else
					{
						//loose life
						pPlayer.LooseLife();
						pPlayer.Repulse( -tCollideResult );

						//reset no damage trophy progression
						TTrophy::ResetScores( EScore_NoDamage );
						
						TSoundSystem::Play( ESound_Hit );

						if( !pPlayer.StillAlive() )
							return ERoomResult_Death;
					}
				}
			}

			if( m_pEnnemies[i]->GetState() == EDynamicState_Dying )
			{
				delete m_pEnnemies[i];
				m_pEnnemies[i] = nullptr;
			}
		}
	}
	return ERoomResult_Playing;
}

ERoomResult TRoom::StaticObjectUpdate( TPlayer & pPlayer )
{
	ItemsUpdate(pPlayer);
	ERoomResult eResult = DoorsUpdate(pPlayer);
	ObstacleUpdate(pPlayer);

	if (eResult != ERoomResult_Playing && eResult != ERoomResult_Death)
		ResetEnnemiesPos();

	return eResult;
}

ERoomResult TRoom::DoorsUpdate( TPlayer & pPlayer )
{
	for (int i = 0; i < ESide_Count; i++)
	{
		if (m_pDoor[i] != nullptr)
		{
			//collision : player enters a door
			TGfxVec2 tCollideResult = m_pDoor[i]->Collide(pPlayer);

			if (tCollideResult.x != 0.f || tCollideResult.y != 0.f)
			{
				EDoor_State eIsDoorLocked = m_pDoor[i]->GetDoorState();

				if( eIsDoorLocked != EDoor_State_Locked )//if door is unlocked
				{
					ESide eEntry = m_eEntrySide;
					ESide eExit = m_eExitSide;

					if (i == eEntry || i == eExit)//entry or exit door
					{
						if(pPlayer.EnterRoom( &m_pDoor[i]->GetOtherSideDoor() ))//enter door with success
						{
							if( i == eEntry )
							{
								GfxDbgPrintf("%d entry\n", m_iId);
								return ERoomResult_EntryDoor;
							}
							else
							{
								GfxDbgPrintf("%d exit\n", m_iId);
								return ERoomResult_ExitDoor;
							}
						}
						else//if door has no other side
						{
							if(i == eExit)
								return ERoomResult_VoidExitDoor;
							else
								pPlayer.Repulse( tCollideResult );
						}
					}
					else//any other door
					{
						if( pPlayer.CheckSelectedAndCollected( EItemType_Locker ) )
						{
							//use item
								GfxDbgPrintf("Locker activated\n");
								m_pDoor[i]->SetDoorState(EDoor_State_Locked);

								pPlayer.YieldActualItem();
								pPlayer.Repulse(tCollideResult);
								
								TSoundSystem::Play( ESound_Locker );
						}
						else
						{
							if( pPlayer.EnterRoom( &m_pDoor[i]->GetOtherSideDoor() ) == false )
							pPlayer.Repulse( tCollideResult*2.0f );
							return ERoomResult_WrongDoor;
						}
					}
				}
				else
				{
					GfxDbgPrintf("%d Door locked !\n", m_iId);
					pPlayer.Repulse(tCollideResult);
				}
				break;//if a collision has been triggered on a door, there is no need to check the others
			}
		}
	}
	return ERoomResult_Playing;
}

void TRoom::ItemsUpdate(TPlayer & pPlayer)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_pItem[i] != nullptr)
		{			
			TGfxVec2 tCollideResult = m_pItem[i]->CollideCC(pPlayer);
			if (tCollideResult.x != 0.f || tCollideResult.y != 0.f)
			{
				EItemType eItemType = m_pItem[i]->GetType();
				bool bCollected = pPlayer.CollectItem( eItemType );

				if (bCollected)
				{
					//remove collectible item
					delete m_pItem[i];
					m_pItem[i] = nullptr;

					TSoundSystem::Play( ESound_Collect );
				}
			}
		}
	}
}

void TRoom::ObstacleUpdate(TPlayer & pPlayer)
{
	for (int i = 0; i < OBSTACLE_MAX; i++)
	{
		if( m_pObstacles[i] != nullptr )
		{
			{
				TGfxVec2 tCollideResult = m_pObstacles[i]->Collide( pPlayer );
				if( tCollideResult.x != 0.f || tCollideResult.y != 0.f )
				{
					pPlayer.Repulse( tCollideResult );
				}
			}

			for( int j = 0; j < ENNEMY_MAX; j++ )
			{
				if( m_pEnnemies[j] != nullptr )
				{
					TGfxVec2 tCollideResult = m_pObstacles[i]->Collide( *m_pEnnemies[j] );
					if (tCollideResult.x != 0.f || tCollideResult.y != 0.f)
					{
						m_pEnnemies[j]->Repulse(tCollideResult);
					}
				}
			}
		}
	}
}

void TRoom::ShowExit()
{
	for( int i = 0; i < ESide_Count; ++i )
	{
		if( i == m_eExitSide )
		{
			m_pDoor[i]->SetDoorState( EDoor_State_Shown );
		}
		else if( i != m_eEntrySide )
		{
			m_pDoor[i]->SetDoorState( EDoor_State_Locked );
		}
	}
}

TRoom * TRoom::FindRoom( int iId )
{
	if( m_iId == iId )
		return this;
	else
	{
		TRoom * pNextRoom = GetNextItem();
		while( pNextRoom != nullptr )
		{
			if( pNextRoom->GetId() == iId )
				return pNextRoom;
			pNextRoom = pNextRoom->GetNextItem();
		}
		return nullptr;
	}
}

void TRoom::InitTextures()
{
	s_pBackgroundTexture[0] = GfxTextureLoad( "first_room_background.tga" );
	s_pBackgroundTexture[1] = GfxTextureLoad( "room_background_1.tga" );
	s_pBackgroundTexture[2] = GfxTextureLoad( "room_background_2.tga" );
}

void TRoom::DestroyTextures()
{
	for( int i = 0; i < BG_COUNT; ++i )
	{
		if( s_pBackgroundTexture[i] != nullptr )
			GfxTextureDestroy( s_pBackgroundTexture[i] );
	}
}

void TRoom::ResetEnnemiesPos()
{
	for (int i = 0; i < ENNEMY_MAX; i++)
	{
		if (m_pEnnemies[i] != nullptr)
		{
			THunter * pHunter = reinterpret_cast< THunter* >( m_pEnnemies[i] );
			EHunterType eType = pHunter->GetType();

			if( eType == EHunterType_Wanderer || eType == EHunterType_Chaser )
			{
				int iCorner = ( i % 4 ) + 1;
				pHunter->ResetPos( iCorner );
			}
		}
	}
}



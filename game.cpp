#include <string>

#include "flib.h"
#include "flib_vec2.h"

#include "controler.h"
#include "texture_controler.h"
#include "element.h"
#include "circle.h"
#include "square.h"
#include "player.h"
#include "door.h"
#include "room.h"
#include "item.h"
#include "sentinel.h"
#include "transition.h"
#include "screen_overlay\screen-overlay.h"
#include "parser\sll_root.h"

#include "game.h"

#include "constants.h"
#include "trophy.h"
#include "sound.h"

TPlayer * g_pPlayer;

TGame * TGame::CreateInstance( int iStartRooms, EGame_State eState, TSetting & pSetting )
{
	if( s_pInstance == nullptr )
	{
		s_pInstance = new TGame( iStartRooms, eState, pSetting );
		return s_pInstance;
	}
	return nullptr;
}

TGame::TGame(int iStartRooms, EGame_State eState, TSetting & pSetting)
	: m_pRoomList(nullptr),
	m_pPlayer(nullptr),
	m_eState(eState),
	m_iRoomCount( 0 ),
	m_pTransitionParam(nullptr),
	m_pScreenOverlay(nullptr),
	m_pSetting(&pSetting),
	m_pMenuButton(nullptr),
	m_pProgressInfo( nullptr )
{
	//init info text
	m_pProgressInfo = new TSquare();
	TTrophy::SetTextOverlay( *m_pProgressInfo );
	//init screen overlay
	m_pScreenOverlay = new TScreenOverlay( EScreenOverlayState_AllBlack, (float)TRANSITION_DELAY_MAX );
	//create room list
	TRoom::InitSettings( &pSetting );
	TRoom::InitTextures();

	//doors & rooms
	m_pRoomList = new TSLL_Root<TRoom>();
	for( int i = 0; i < iStartRooms; i++ )
	{
		AddRoom();
	}
	
	//player
	TRoom * pFirstRoom = m_pRoomList->GetFirstItem();
	m_pPlayer = new TPlayer( TGfxVec2( 0.f, 0.f ), 0.f, fPLAYER_RADIUS, pFirstRoom );
	m_pPlayer->SetToDoorPos( pFirstRoom->GetEntryDoor() );
	//Set Global variable
	g_pPlayer = m_pPlayer;

	//menu button
	TTileInfo tButtonSize = TTextureControler::GetTileSize( ETextureData_MenuButton );
	m_pMenuButton = new TSquare(
		TGfxVec2( 
			SCREEN_SIZE_X - ( float ) tButtonSize.x/2,
			SCREEN_SIZE_Y - ( float ) tButtonSize.y
		),
		0.f,
		TGfxVec2( ( float ) tButtonSize.x/2, ( float ) tButtonSize.y/2 ),
		ETextureData_MenuButton );
}

TGame::~TGame()
{
	delete m_pRoomList;

	delete m_pPlayer;

	delete m_pScreenOverlay;

	delete m_pTransitionParam;

	delete m_pMenuButton;

	TRoom::DestroyTextures();

	delete m_pProgressInfo;

	TTrophy::ResetScores( EScore_All );
}

bool TGame::Update()
{
	switch( m_eState )
	{
	case EGame_State_Playing:
	{
		//player
		m_pPlayer->Update();
		
		//Room
		TRoom & pCurrentRoom = m_pPlayer->GetCurrentRoom();
		const int iPrecScore = m_pPlayer->GetScore();//save the score before any upgrade
		ERoomResult eResult = pCurrentRoom.GameplayUpdate(*m_pPlayer);
		
		//Gameplay result
		if( eResult == ERoomResult_VoidExitDoor )
		{
			AddRoom();
			const TDoor & pDoor = pCurrentRoom.GetExitDoor();
			m_pPlayer->EnterRoom( &pDoor.GetOtherSideDoor() );
			eResult = ERoomResult_ExitDoor;
		}
		
		if( eResult == ERoomResult_EntryDoor || eResult == ERoomResult_ExitDoor )
		{
			//progression feedback
			if( m_pProgressInfo->IsWaiting() )
				m_pProgressInfo->SetDelay( 0, false );

			const int iRoomId = m_pPlayer->GetCurrentRoom().GetId();

			if( iRoomId - 2 == iPrecScore &&//condition reached at first entry in a room only (can't work if the score can decrease at some point)
				iRoomId % FEEDBACK_RANGE == 0 )//if the room id is a multiple of FEEDBACK_RANGE (feedback every X room)
			{
				char pText[TEXT_ID_SIZE];
				sprintf(pText, "Congratulation\nYou reached\nroom %d", iRoomId);
				m_pProgressInfo->ActivateText( pText, TEXT_INFO_DURATION );
			}
			
			TRoom * pPrecRoom;

			if( eResult == ERoomResult_EntryDoor )
			{
				//get precedent room for transition
				pPrecRoom = &pCurrentRoom.GetEntryDoor().GetOtherSideDoor().GetRoom();
			}
			else
			{
				pPrecRoom = &pCurrentRoom.GetExitDoor().GetOtherSideDoor().GetRoom();
				//trophy update
				TTrophy::UpdateScore();
			}

			//init transition
			m_pTransitionParam = new TTransition( eResult, pCurrentRoom, *pPrecRoom, TRANSITION_DELAY_MAX );
			m_eState = EGame_State_RoomTransition;
			GfxDbgPrintf( "start transition\n" );
			
			TSoundSystem::Play( ESound_DoorSuccess );
		}
		else if( eResult == ERoomResult_Death )
		{
			//update state
			m_eState = EGame_State_Death;
			//Set feedback text
			m_pProgressInfo->ActivateText( "Game Over", TEXT_INFO_DURATION );

			TSoundSystem::Play( ESound_GameOver );
			return true;
		}
		else if (eResult == ERoomResult_WrongDoor)
		{
			//start fade
			m_pScreenOverlay->Start(EScreenOverlayState_FadingIn);
			//failure feedback
			m_pProgressInfo->ActivateText( "Wrong Door\nReTry !", TEXT_INFO_DURATION );
			//Reset Trophy's progression
			TTrophy::ResetScores( EScore_All );

			TSoundSystem::Play( ESound_DoorFailure );
		}

		if (m_pScreenOverlay->GetState() != EScreenOverlayState_AllTranslucent)
		{
			if (m_pScreenOverlay->GetState() == EScreenOverlayState_AllBlack)
				m_pScreenOverlay->Start(EScreenOverlayState_FadingIn);

			m_pScreenOverlay->Update();
		}
	}
		break;
	case EGame_State_RoomTransition:
	{
		if (m_pTransitionParam != nullptr)
		{
			EGame_State eState = m_pTransitionParam->Update();

			if( eState == EGame_State_Playing )
			{
				delete m_pTransitionParam;
				m_pTransitionParam = nullptr;

				m_eState = EGame_State_Playing;

				//m_pProgressInfo->Desactivate();//tmp

				GfxDbgPrintf( "end transition\n" );
			}
			else if( eState == EGame_State_Pause )
			{
				//???
			}	
		}
		else
		{
			m_eState = EGame_State_Playing;
		}
	}
		break;
	case EGame_State_Pause:
	{
		//???
	}
		break;
	case EGame_State_Death:
	{
		m_pPlayer->Update();
		TRoom & pCurrentRoom = m_pPlayer->GetCurrentRoom();
		pCurrentRoom.GameplayUpdate( *m_pPlayer );
		
		if( !m_pPlayer->IsWaiting() )
		{
			GfxDbgPrintf( "player is dead\n" );
			m_pScreenOverlay->Start(EScreenOverlayState_FadingOut);
			m_eState = EGame_State_ToMenu;
		}
		return true;
	}
		break;
	case EGame_State_ToMenu:
	{
		m_pScreenOverlay->Update();

		if (m_pScreenOverlay->GetState() == EScreenOverlayState_AllBlack)
		{
			return false;
		}
		return true;
	}
		break;
	}

	//update feedback text
	m_pProgressInfo->UpdateText();

	//test menu button
	if( GfxInputIsJustReleased( EGfxInputID_MouseLeft ) )
	{
		int iMouseX = GfxGetCurrentMouseX();
		int iMouseY = GfxGetCurrentMouseY();
		if (m_pMenuButton->CollidePoint(TGfxVec2((float)iMouseX, (float)iMouseY)))
		{
			m_eState = EGame_State_ToMenu;
			m_pScreenOverlay->Start(EScreenOverlayState_FadingOut);
			TSoundSystem::Play( ESound_MenuClick );
		}
	}
	return true;
}

void TGame::Render()
{	
	switch( m_eState )
	{
	case EGame_State_ToMenu:
	case EGame_State_Death:
	case EGame_State_Playing:
	{
		TRoom * pCurrentRoom = &m_pPlayer->GetCurrentRoom();
		if( pCurrentRoom != nullptr )
			pCurrentRoom->Render( m_eState );

		if( m_pPlayer != nullptr )
			m_pPlayer->Render();

		m_pProgressInfo->RenderText();
		m_pMenuButton->Render();
	
		if (m_pScreenOverlay->GetState() != EScreenOverlayState_AllTranslucent)
			m_pScreenOverlay->Render();
	}
		break;
	case EGame_State_RoomTransition:
	{
		if( m_pTransitionParam != nullptr )
		m_pTransitionParam->Render();
	}
		break;
	case EGame_State_Pause:
	{

	}
		break;
	default:
		break;
	}

}

void TGame::AddRoom()
{
	TRoom * pLastRoom = m_pRoomList->FindLastItem();

	// Sides:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	ESide eEntrySide;
	ESide eExitSide;

	//entry == exit of prec
	if( pLastRoom == nullptr)//no prec (first room)
	{
		eEntrySide = ESide_Down;
	}
	else
	{
		ESide ePrecExitSide = pLastRoom->GetExitSide();

		if( ePrecExitSide % 2 == 0 )//up or left
			eEntrySide = static_cast<ESide>( ePrecExitSide + 1 );//get dwn or right
		else//dwn or right
			eEntrySide = static_cast<ESide>( ePrecExitSide - 1 );//get up or left
	}

	//exit == random but not the same as the entry
	do
	{
		eExitSide = static_cast< ESide >( GfxMathGetRandomInteger( 0, ESide_Count - 1 ) );
	} while( eExitSide == eEntrySide );

	//create room :::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	TRoom * pNewRoom = new TRoom( eEntrySide, eExitSide, ++m_iRoomCount );

	m_pRoomList->Append( pNewRoom );

	//Bound Doors :::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if( m_iRoomCount-1 > 0 )//exept for 1st room
	{
		//Bound entry and exit doors
		TDoor & pEntryDoor = pNewRoom->GetEntryDoor();
		TDoor & pPrecExitDoor = pLastRoom->GetExitDoor();
		BoundDoors( pEntryDoor, pPrecExitDoor );

		//link all other doors to first
		const TDoor * pFirstDoor = &m_pRoomList->GetFirstItem()->GetEntryDoor();//save first room entry door

		for( int j = 0; j < ESide_Count; ++j )
		{
			if( j != eEntrySide && j != eExitSide )
			{
				TDoor * pDoor = pNewRoom->GetDoor( j );
				pDoor->SetOtherSideDoor( *pFirstDoor );
			}
		}
	}
}

void TGame::BoundDoors( TDoor & pDoor1, TDoor & pDoor2 )
{
	pDoor1.SetOtherSideDoor( pDoor2 );
	pDoor2.SetOtherSideDoor( pDoor1 );
}


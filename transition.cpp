#include "flib.h"
#include "flib_vec2.h"

#include "room.h"
#include "door.h"
#include "wait.h"
#include "transition.h"

TTransition::TTransition( ERoomResult eTransitionType, TRoom & pCurrentRoom, TRoom & pOtherRoom, int iDelay )
	:m_tDir(TGfxVec2(0.f, 0.f)),
	m_eTransitionType(eTransitionType),
	m_pCurrentRoom(nullptr),
	m_pOtherRoom(nullptr),
	m_pTimer(nullptr),
	m_eState(ETransitionState_DoorOpening)
{
	//init
	m_pCurrentRoom = &pCurrentRoom;
	m_pOtherRoom = &pOtherRoom;
	m_pTimer = new TWait( 0, iDelay, false );

	//settup
	TGfxVec2 tPrecDoorPos;
	TGfxVec2 tNextDoorPos;

	if( m_eTransitionType != ERoomResult_EntryDoor )
	{
		tNextDoorPos = m_pCurrentRoom->GetExitDoor().GetPos();
		tPrecDoorPos = m_pOtherRoom->GetEntryDoor().GetPos();
	}
	else
	{
		tNextDoorPos = m_pCurrentRoom->GetEntryDoor().GetPos();
		tPrecDoorPos = m_pOtherRoom->GetExitDoor().GetPos();
	}

	m_tDir = (tPrecDoorPos - tNextDoorPos).Normalize();
}

TTransition::~TTransition()
{
	m_pCurrentRoom = nullptr;
	m_pOtherRoom = nullptr;
	delete m_pTimer;
}

EGame_State TTransition::Update()
{
	switch( m_eState )
	{
	case ETransitionState_DoorClosing:
	{
		TDoor * pDoor = nullptr;

		//get the opposite door to animate
		if( m_eTransitionType == ERoomResult_EntryDoor )
			pDoor = &m_pOtherRoom->GetExitDoor();
		else if( m_eTransitionType == ERoomResult_ExitDoor )
			pDoor = &m_pOtherRoom->GetEntryDoor();

		//check door anim progression
		int iAnimStep = pDoor->GetAnimStep();

		if( iAnimStep == TTextureControler::GetTileCountX( ETextureData_Door ) - 1 )// anim begining
		{
			pDoor->SetAnimPlay( true );
		}
		else if( iAnimStep == 0 )//anim ending
		{
			pDoor->SetAnimPlay( false );
			pDoor->SetAnimReverse( false );

			return EGame_State_Playing;
		}

		pDoor->PlayAnim();

	}
		break;
	case ETransitionState_DoorOpening:
	{
		TDoor * pDoor = nullptr;

		//get the door to animate
		if( m_eTransitionType == ERoomResult_EntryDoor )
			pDoor = &m_pCurrentRoom->GetEntryDoor();
		else if(m_eTransitionType == ERoomResult_ExitDoor )
			pDoor = &m_pCurrentRoom->GetExitDoor();

		//check door anim progression
		int iAnimStep = pDoor->GetAnimStep();

		if( iAnimStep == 0 )//anim begining
		{
			pDoor->SetAnimPlay( true );
			pDoor->SetAnimReverse( false );
		}
		else if( iAnimStep == TTextureControler::GetTileCountX( ETextureData_Door )-1 )// anim ending
		{
			pDoor->SetAnimPlay( false );

			m_eState = ETransitionState_RoomTransit;
			int iTime = GfxTimeGetMilliseconds();
			m_pTimer->SetStartTime( iTime );
			m_pTimer->SetActive( true );

			//set opposite door to closing
			TDoor * OppositepDoor = nullptr;

			if( m_eTransitionType == ERoomResult_EntryDoor )
				OppositepDoor = &m_pOtherRoom->GetExitDoor();
			else if( m_eTransitionType == ERoomResult_ExitDoor )
				OppositepDoor = &m_pOtherRoom->GetEntryDoor();

			OppositepDoor->SetTile( TTextureControler::GetTileCountX( ETextureData_Door ) - 1, OppositepDoor->GetAnimLevel() );
			OppositepDoor->SetAnimReverse( true );
		}

		pDoor->PlayAnim();
	}
		break;
	case ETransitionState_RoomTransit:
	{
		//lerp during transition delay
		if( m_pTimer->Update() == true )
		{ 
			float fProgression = m_pTimer->GetProgress();

			int iDistance = 0;
			//define the distance of the transition depending of if the transition is vertical or horizontal
			if (m_tDir.x > 0.f || m_tDir.x < 0.f)
				iDistance = SCREEN_SIZE_X;
			else
				iDistance = SCREEN_SIZE_Y;

			//set room position
			float fStep = fProgression * iDistance;
			m_pOtherRoom->TransitionUpdate( m_tDir * ( fStep - iDistance ) );
			m_pCurrentRoom->TransitionUpdate(m_tDir * fStep);
		}
		else
		{
			//reset room position when transition ends
			m_pOtherRoom->TransitionUpdate(TGfxVec2(fBACKGROUND_POS_X, fBACKGROUND_POS_Y));
			m_pCurrentRoom->TransitionUpdate(TGfxVec2(fBACKGROUND_POS_X, fBACKGROUND_POS_Y));
			
			//reset door sprite
			TDoor * pDoor = nullptr;

			if( m_eTransitionType == ERoomResult_EntryDoor )
				pDoor = &m_pCurrentRoom->GetEntryDoor();
			else if( m_eTransitionType == ERoomResult_ExitDoor )
				pDoor = &m_pCurrentRoom->GetExitDoor();

			pDoor->SetTile( 0, pDoor->GetAnimLevel() );

			m_eState = ETransitionState_DoorClosing;
		}
	}
		break;
	}
	return EGame_State_RoomTransition;
}

void TTransition::Render()
{
	if(m_eState != ETransitionState_DoorOpening)
		m_pOtherRoom->Render( EGame_State_RoomTransition );
	if( m_eState != ETransitionState_DoorClosing )
		m_pCurrentRoom->Render( EGame_State_RoomTransition );
}


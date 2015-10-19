#include "flib.h"
#include "flib_vec2.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "door.h"
#include "room.h"
#include "hud.h"
#include "wait.h"
#include "item.h"

#include "player.h"
#include "square.h"
#include "trophy.h"
#include "sound.h"

#include "constants.h"

TPlayer::TPlayer( const TGfxVec2 & tPos, float fOrient, float fRadius, TRoom * pCurrentRoom )
	: TDynamic( tPos, fOrient, fRadius, fDEFAULT_SPEED ),
	m_tDest( TGfxVec2( 0.f, 0.f ) ),
	m_eSelectedItem( EItemType_Empty ),
	m_pCurrentRoom( pCurrentRoom ),
	m_pHUD( THUD::CreateHUD( pCurrentRoom->GetId() ) ),
	m_iScore( 0 ),
	m_cLifePoint( LIFE_MAX )
{
	SpriteSettup(ETextureData_Player, TGfxVec2(m_fRadius, m_fRadius));

	//inventory
	for( int i = 0; i < EItemType_Count; i++ )
	{
		m_bInventory[i] = false;
	}

	//consumed Items
	for( int i = 0; i < EItemType_Count - 1; i++ )
	{
		m_pConsumedItem[i] = nullptr;
	}
}

TPlayer::~TPlayer()
{
	THUD::DestroyHUD();

	for( int i = 0; i < EItemType_Count - 1; i++ )
	{
		delete m_pConsumedItem[i];
	}
}

void TPlayer::Update()
{
	if( m_eState != EDynamicState_Dying )
	{
		GetInput();
		Move();
	}
	switch( m_eState )
	{
	case EDynamicState_Hurt:
		Hurt();//update delay when hurt
		break;
	case EDynamicState_Dying:
		Die();
		break;
	default:
		break;
	}

	PlayAnim();
	UpdateConsumedItemFeedBack();
	m_pHUD->UpdateLights();
}

void TPlayer::Render()
{
	for( int i = 0; i < EItemType_Count - 1; i++ )
	{
		if( m_pConsumedItem[i] != nullptr )
			m_pConsumedItem[i]->Render();
	}

	TElement::Render();
	m_pHUD->Render();
}

void TPlayer::Move()
{
	if ( !ReachPoint(m_tDest) )
	{
		if (GetAnimLevel() != EDynamicAnimState_Walking)
			SetAnimLevel(EDynamicAnimState_Walking);
		TDynamic::Move();
	}
	else
	{
		if (GetAnimLevel() != EDynamicAnimState_Idle)
			SetAnimLevel(EDynamicAnimState_Idle);
	}
}

void TPlayer::Repulse( const TGfxVec2 & tPos )
{
	TDynamic::Repulse(tPos);
	m_tDest = m_tPos;
	//m_tDir = TGfxVec2( 0.f, 0.f );
}

void TPlayer::GetInput()
{
	if( GfxInputIsJustReleased( EGfxInputID_MouseLeft ) )
	{
		EItemType eType = (EItemType)m_pHUD->CheckInput();
		
		//click outside of buttons
		if( (int)eType == BUTTON_NULL )
		{
			const float fMouseX = ( float ) GfxGetCurrentMouseX();
			const float fMouseY = ( float ) GfxGetCurrentMouseY();

			if( fMouseX > fPLAYER_INPUT_LIMIT &&
				fMouseX < SCREEN_SIZE_X - fPLAYER_INPUT_LIMIT )
			{
				m_tDest = TGfxVec2(fMouseX, fMouseY);
				m_tDir = (m_tDest - m_tPos).Normalize();

				SetOrient( m_tDir );
			}
		}
		else//into buttons
		{
			
			EItemType eActualSelectedItem = m_eSelectedItem;
			EDoor_State eExitDoor_State = m_pCurrentRoom->GetExitDoor().GetDoorState();

			//gameplay// click on key item to use it
			if (eType == EItemType_Key &&
				CheckSelectedAndCollected( eType ) &&//if item is already selected & is collected
				( eExitDoor_State != EDoor_State_Locked &&
				eExitDoor_State != EDoor_State_Shown ) )//and door isn't already shown
			{
				YieldActualItem();
				m_pCurrentRoom->ShowExit();
				m_pHUD->UpdateButton( eType, EButtonAnimState_Unavailable_Selected, false );

				TSoundSystem::Play( ESound_Key);
			}
			else
			{
				m_eSelectedItem = eType;

				if( eType != eActualSelectedItem )
				{
				//new selection
				if( m_bInventory[eType] == true )
					m_pHUD->UpdateButton( eType, EButtonAnimState_Available_Selected, false );
				else
					m_pHUD->UpdateButton( eType, EButtonAnimState_Unavailable_Selected, false );

				//precedent selection
					if( m_bInventory[eActualSelectedItem] == true )
						m_pHUD->UpdateButton( eActualSelectedItem, EButtonAnimState_Available, false );
					else
						m_pHUD->UpdateButton( eActualSelectedItem, EButtonAnimState_Unavailable, false );
				}

				TSoundSystem::Play( ESound_HudClick );
			}
		}
	}
}

bool TPlayer::EnterRoom( TDoor const * tDoor )
{
	if( tDoor != nullptr )
	{
		//update room
		m_pCurrentRoom = &tDoor->GetRoom();
		//go to door pos
		SetToDoorPos( *tDoor );

		//update score
		if( m_pCurrentRoom->GetId()-1 > m_iScore )
			m_iScore++;

		//update hud text
		m_pHUD->UpdateRoomTextInfo( m_pCurrentRoom->GetId(), m_iScore );
		
		//reset item consumption feedback
		for( int i = 0; i < EItemType_Count - 1; i++ )
		{
			delete m_pConsumedItem[i];
			m_pConsumedItem[i] = nullptr;
		}

		return true;
	}
	else
		return false;
}

void TPlayer::SetToDoorPos(const TDoor & pDoor)
{
	TGfxVec2 tDoorPos = pDoor.GetPos();
	TGfxVec2 tScreenCenter = TGfxVec2(SCREEN_SIZE_X / 2.f, SCREEN_SIZE_Y / 2.f);
	TGfxVec2 tDoorDir = (tScreenCenter - tDoorPos).Normalize();
	float fDoorHeight = pDoor.GetRadius().y;

	TGfxVec2 tPos = tDoorPos + tDoorDir * (fDoorHeight + m_fRadius);
	
	//set pos
	SetPos( tPos );

	//reset dir
	m_tDir = TGfxVec2(0.f, 0.f);

	//reset dest
	m_tDest = m_tPos;

	//set orient
	SetOrient( atan2f( tDoorDir.y, tDoorDir.x ) );
}

EItemType TPlayer::YieldActualItem()
{
	int i = m_eSelectedItem;

	if (m_bInventory[i] == true)
	{
		//update inventory
		m_bInventory[i] = false;
		//update hud
		m_pHUD->UpdateButton( i, EButtonAnimState_Unavailable_Selected, false);

		//consumed item feed back
		if( m_eSelectedItem > EItemType_Empty )//avoid inventory's empty slot
		{
			m_pConsumedItem[i-1] = new TItem( m_tPos, fITEMRADIUS / 2.f, EItemType( i ) );
			//m_pConsumedItem[i - 1]->SetTile( EButtonAnimState_Unavailable_Selected, i );
			m_pConsumedItem[i-1]->SetDelay( 2000, true );
		}

		//reset no item trophy progression
		TTrophy::ResetScores( EScore_NotItem );

		return m_eSelectedItem;
	}
	return EItemType_Null;
}

bool TPlayer::CollectItem(const EItemType eItem)
{
	if (m_bInventory[eItem] == false)
	{
		//GfxDbgPrintf("%d %d %d %d\n", CheckInventory(EItemType_Sword), CheckInventory(EItemType_Locker), CheckInventory(EItemType_Key), CheckInventory(EItemType_CheckPoint));
		//update hud
		if (eItem == m_eSelectedItem)
			m_pHUD->UpdateButton( eItem, EButtonAnimState_Available_Selected, true);
		else
			m_pHUD->UpdateButton( eItem, EButtonAnimState_Available, true );

		return m_bInventory[eItem] = true;
	}
	return false;
}

void TPlayer::LooseLife()
{
	m_cLifePoint--;

	if( m_cLifePoint < 0 || m_cLifePoint > LIFE_MAX)
	{
		//security so life doesn't go under 0 (shoudn't be happening)
		m_cLifePoint = 0;
	}
	else
	{
		//start delay
		if( m_pDelay != nullptr )
			delete m_pDelay;

		m_pDelay = new TWait();
		
		if( StillAlive() )
		{
			m_pDelay->Start( HEALING_DURATION ); 
			m_eState = EDynamicState_Hurt;
		}
		else
		{
			m_pDelay->Start( DYING_DURATION );
			m_eState = EDynamicState_Dying;
			StartAnim( EDynamicAnimState_Death, true );
		}
	}

	//update hud
	m_pHUD->UpdateLifePoints( m_cLifePoint );
}

void TPlayer::Hurt()
{
	if( m_pDelay->IsActive() )
	{
		m_pDelay->Update();

		int iProgression = int( m_pDelay->GetProgress() * 10.f ) % 2;

		if( iProgression % 2 == 0 )
			GfxSpriteSetColor( m_pSprite, GfxColor( 255, 255, 255, 128 ) );
		else
			GfxSpriteSetColor( m_pSprite, EGfxColor_White );

		if( !m_pDelay->IsActive() )//last time before ends
		{
			GfxSpriteSetColor( m_pSprite, EGfxColor_White );

			m_eState = EDynamicState_Active;
		}
	}
	else
	{
		delete m_pDelay;
		m_pDelay = nullptr;
	}
}

bool TPlayer::StillAlive()const
{
	if( m_cLifePoint > 0 )
		return true;
	return false;
}

void TPlayer::Die()
{
	//delay update
	if( m_pDelay->IsActive() )
		m_pDelay->Update();
	else
	{
		delete m_pDelay;
		m_pDelay = nullptr;
	}

	//TDynamic::Die();
}

void TPlayer::UpdateConsumedItemFeedBack()
{
	for( int i = 0; i < EItemType_Count - 1; i++ )
	{
		if( m_pConsumedItem[i] != nullptr )
		{
			TWait * pDelay = m_pConsumedItem[i]->GetDelay();

			pDelay->Update();

			if( pDelay != nullptr && pDelay->IsActive() == true )
			{
				//color
				TGfxSprite * pSprite = m_pConsumedItem[i]->GetSprite();
				float fLevel = pDelay->GetProgress();
				GfxSpriteSetColor( pSprite, GfxColor(0,0,0, ( 1.f - fLevel ) * 255 ) );

				//sprite offset pos
				TGfxSprite * pItemSprite = m_pConsumedItem[i]->GetSprite();
				TGfxVec2 tSpritePos = m_pConsumedItem[i]->GetPos() - TGfxVec2( 0.f, fCONSUMED_ITEM_RANGE * fLevel );
				GfxSpriteSetPosition( pItemSprite, tSpritePos.x, tSpritePos.y );
			}
			else
			{
				delete m_pConsumedItem[i];
				m_pConsumedItem[i] = nullptr;
			}
		}
	}
}


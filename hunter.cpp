#define _USE_MATH_DEFINES
#include <math.h>

#include "flib.h"
#include "flib_vec2.h"

#include "player.h"
#include "wait.h"

#include "hunter.h"
#include "constants.h"
#include "game.h"

extern TPlayer * g_pPlayer;

THunter::THunter( int iCorner, float fRadius, float fSpeed, EHunterType eType )
	: TDynamic( TGfxVec2(0.f, 0.f), 0.f, fRadius, fSpeed ), m_eType( eType )
{
	//Sprite settup
	ETextureData eData = ETextureData_Chaser;
	if( eType == EHunterType_Wanderer )
		eData = ETextureData_Wanderer;
	
	SpriteSettup( eData, TGfxVec2( m_fRadius, m_fRadius ) );
	SetTile( 0, 0 );

	ResetPos( iCorner );
}

THunter::~THunter()
{

}

void THunter::Update()
{
	switch( m_eType )
	{
	case EHunterType_Wanderer:
		WandererUpdate();
		break;
	case EHunterType_Chaser:
		ChaserUpdate();
		break;
	}

	TDynamic::Update();
}

void THunter::Repulse( const TGfxVec2 & tPos )
{
	TDynamic::Repulse( tPos );

	//Reaction to collision specific to some ennemies
	ChangeDir();
}

//random direction
void THunter::WandererUpdate()
{
	if( m_pDelay == nullptr || m_pDelay->IsActive() == false )
	{
		//reset delay
		const int iDelay = GfxMathGetRandomInteger( WANDER_TRAVEL_MIN, WANDER_TRAVEL_MAX );
		SetDelay( iDelay, true );

		ChangeDir();
	}
	else
	{
		m_pDelay->Update();
		//GfxDbgPrintf( "wanderer update\n" );
	}
}

//follow the player
void THunter::ChaserUpdate()
{
	//chasers follow their prey only by using cardinal axis.
	TGfxVec2 tPreyPos = g_pPlayer->GetPos();

	TGfxVec2 tDist = tPreyPos - m_tPos;

	//The chosen axis is the one that represent the longuest distance to travel ()
	if( tDist.x >= tDist.y )
	{
		if( tDist.y > 0.01f || tDist.y < -0.01f )
			tDist = tDist * TGfxVec2( 0.f, 1.f );
		else
			tDist = tDist * TGfxVec2( 1.f, 0.f );
	}
	else
	{
		if( tDist.x > 0.01f || tDist.x < -0.01f )
			tDist = tDist * TGfxVec2( 1.f, 0.f );
		else
			tDist = tDist * TGfxVec2( 0.f, 1.f );
	}

	SetDir( tDist );
	SetOrient( tDist );
}

void THunter::ResetPos( int iCorner )
{
	if( m_eType == EHunterType_Wanderer )
	{
		//set hunter in a corner of the room
		TGfxVec2 tCenter = TGfxVec2( SCREEN_SIZE_X / 2.f, SCREEN_SIZE_Y / 2.f );
		float fAngle = float( M_PI_4 + M_PI_2 * iCorner );
		TGfxVec2 tAngle = TGfxVec2( cosf( fAngle ), sinf( fAngle ) );

		TGfxVec2 tNewPos = tCenter + tAngle * 100.f;

		SetPos( tNewPos );
	}
	else if( m_eType == EHunterType_Chaser )
	{
		//set the hunter in the center of the room
		TGfxVec2 tCenter = TGfxVec2( SCREEN_SIZE_X / 2.f, SCREEN_SIZE_Y / 2.f );
		SetPos( tCenter );
	}
}


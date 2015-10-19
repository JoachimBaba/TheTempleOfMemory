#define _USE_MATH_DEFINES
#include <math.h>

#include "flib.h"
#include "flib_vec2.h"

#include "player.h"
#include "wait.h"

#include "dynamic.h"
//#include "constants.h"

TDynamic::TDynamic( const TGfxVec2 & tPos, float fOrient, float fRadius, float fSpeed )
	: TCircle( tPos, fOrient, fRadius ), m_tDir( TGfxVec2( 1.f, 0.f ) ), m_fSpeed( fSpeed ), m_eState( EDynamicState_Active )
{
}

TDynamic::~TDynamic()
{

}

void TDynamic::Update()
{
	switch( m_eState )
	{
	case EDynamicState_Active:
		Move();
		break;
	case EDynamicState_Hurt:
		Die();
		break;
	default:
		break;
	}

	PlayAnim();
}

void TDynamic::Move()
{
	float fDelta = GfxTimeFrameGetAverageDurationInSeconds();

	if (m_tDir.x != 0.f || m_tDir.y != 0.f)
	{
		if (m_tDir.Length() > 1.f)
			m_tDir = m_tDir.Normalize();

		TGfxVec2 tNewPos = m_tPos + m_tDir * (m_fSpeed * fDelta);
		m_tPos = tNewPos;
		GfxSpriteSetPosition(m_pSprite, m_tPos.x, m_tPos.y);
	}
}

bool TDynamic::ReachPoint(const TGfxVec2 & tDest)
{
	float fDelta = GfxTimeFrameGetAverageDurationInSeconds();
	float fStep = m_fSpeed * fDelta;

	if( ( tDest - m_tPos ).SquaredLength() < fStep * fStep )
		return true;
	return false;
}

void TDynamic::Repulse(const TGfxVec2 & tPos)
{
	SetPos( m_tPos + tPos );
}

void TDynamic::ChangeDir()
{
	float fNewDir = 0.f;
	float fX = 0.f;
	float fY = 0.f;

	TGfxVec2 tNewDir = TGfxVec2( 0.f, 0.f );	
	/*
	fNewDir = ( float ) M_PI_2 * GfxMathGetRandomInteger( 0, 3 );
	fX = cosf( fNewDir );
	fY = sinf( fNewDir );

	tNewDir = TGfxVec2( fX, fY );
	*/
	
	float fDot = 0.f;
	do
	{
		fNewDir = ( float ) M_PI_2 * GfxMathGetRandomInteger( 0, 3 );
		fX = cosf( fNewDir );
		fY = sinf( fNewDir );

		tNewDir = TGfxVec2( fX, fY );

		fDot = m_tDir.DotProduct( tNewDir );
	}
	while( fDot > 0.f );//security that assure that the new direction isn't the same as the actual
	
	SetDir(tNewDir);
	SetOrient( tNewDir );
}

void TDynamic::Die()
{
	//check if anim
	if( GetAnimPlay() == false )
		m_eState = EDynamicState_Dying;
}



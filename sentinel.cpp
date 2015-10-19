#include "flib.h"
#include "flib_vec2.h"
#define _INCLUDE_MATH_DEFINES
#include <math.h>

#include "sentinel.h"
#include "player.h"

#include "constants.h"

TSentinel::TSentinel(const TGfxVec2 & tPos, float fOrient, float fRadius)
: TDynamic(tPos, fOrient, fRadius, fHUNTER_SPEED), m_iCurrentDest(0), m_iDestCount(0)
{
	//Sprite settup
	SpriteSettup(ETextureData_Sentinel, TGfxVec2(m_fRadius, m_fRadius));
	SetTile(0, 0);
}

TSentinel::TSentinel(const TGfxVec2 & tPos, float fOrient, float fRadius, TGfxVec2 * pDests, int iDestsCount, int iCurrentDest)
	: TDynamic( tPos, fOrient, fRadius, fHUNTER_SPEED ), m_iCurrentDest( 0 ), m_iDestCount( 0 )
{
	//Sprite settup
	SpriteSettup( ETextureData_Sentinel, TGfxVec2( m_fRadius, m_fRadius ) );
	SetTile(0, 0);
	InitDests(pDests, iDestsCount, iCurrentDest);
}

TSentinel::~TSentinel()
{

}

void TSentinel::Move()
{
	if( ReachPoint( m_tDests[m_iCurrentDest] ) )
	{
		m_iCurrentDest++;

		if (m_iCurrentDest >= m_iDestCount)
			m_iCurrentDest = 0;

		m_tDir = m_tDests[m_iCurrentDest] - m_tPos;

		SetOrient( m_tDir );
	}

	TDynamic::Move();
}

void TSentinel::Repulse( const TGfxVec2 & tPos )
{
	TDynamic::Repulse( tPos );

	TGfxVec2 tDest = m_tDests[m_iCurrentDest];
	
	m_tDir = tDest - m_tPos;
}

void TSentinel::InitDests(TGfxVec2 * pDests, int iDestsCount, int iCurrentDest)
{
	m_iDestCount = iDestsCount;

	int iNextDest = iCurrentDest + 1;
	m_iCurrentDest = iNextDest;

	if( iNextDest >= m_iDestCount )
		m_iCurrentDest = 0;

	for (int i = 0; i < iDestsCount; i++)
	{
		m_tDests[i] = pDests[i];
	}

	SetPos( pDests[iCurrentDest] );
	m_tDir = pDests[iNextDest] - m_tPos;
	SetOrient( m_tDir );
}



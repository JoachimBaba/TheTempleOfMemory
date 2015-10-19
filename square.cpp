#include "flib.h"
#include "flib_vec2.h"
#define _INCLUDE_MATH_DEFINES
#include <math.h>

#include "circle.h"
#include "wait.h"
#include "square.h"

#include "constants.h"

TSquare::TSquare()//text overlay constructor
	: TElement( TGfxVec2( SCREEN_SIZE_X / 2.f, SCREEN_SIZE_Y / 2.f ), 0.f ),
	m_tAxisX( TGfxVec2( 0.f, 0.f ) ),
	m_tAxisY( TGfxVec2( 0.f, 0.f ) ),
	m_tRadius( TGfxVec2( 0.f, 0.f ) )
{
	m_pSprite = GfxTextSpriteCreate();
	GfxSpriteSetFilteringEnabled( m_pSprite, false );

	m_pDelay = TWait::CreateDelay();
}

TSquare::TSquare( const TGfxVec2 & tPos )//text overlay constructor
	: TElement( tPos, 0.f ),
	m_tAxisX( TGfxVec2( 0.f, 0.f ) ),
	m_tAxisY( TGfxVec2( 0.f, 0.f ) ),
	m_tRadius( TGfxVec2( 0.f, 0.f ) )
{
	m_pSprite = GfxTextSpriteCreate();
	GfxSpriteSetFilteringEnabled( m_pSprite, false );

	m_pDelay = TWait::CreateDelay();
}

TSquare::TSquare( const TGfxVec2 & tPos, float fOrient, const TGfxVec2 & tRadius)
	: TElement(tPos, fOrient),
	m_tAxisX( TGfxVec2( 0.f, 0.f ) ),
	m_tAxisY( TGfxVec2( 0.f, 0.f ) ),
	m_tRadius( tRadius )
{
	InitAxis();
}

TSquare::TSquare( const TGfxVec2 & tPos, float fOrient, const TGfxVec2 & tRadius, ETextureData eTexture )
	: TElement( tPos, fOrient ),
	m_tAxisX( TGfxVec2( 0.f, 0.f ) ),
	m_tAxisY( TGfxVec2( 0.f, 0.f ) ),
	m_tRadius( tRadius )
{
	InitAxis();
	SpriteSettup( eTexture, m_tRadius );
}

TSquare::~TSquare()
{

}

void TSquare::Update()
{
//	Move();
//	Draw();
}

bool TSquare::UpdateText()
{
	if( m_pDelay->IsActive() )
	{
		m_pDelay->Update();
		return true;
	}
	else
		return false;
}

void TSquare::Render()
{
	TElement::Render();
}

void TSquare::RenderText()
{
	if( m_pDelay->IsActive() )
		GfxTextSpriteRender( m_pSprite, m_tPos.x, m_tPos.y, EGfxColor_White, fFEEDBACK_SIZE, true, true );
}

void TSquare::InitAxis()
{
	float fRadOrient = GetOrient();
	//Axis X
	m_tAxisX.x = m_tRadius.x * cosf( fRadOrient );
	m_tAxisX.y = m_tRadius.x * sinf( fRadOrient );

	//Axis Y
	m_tAxisY.x = m_tRadius.y  * -sinf( fRadOrient );
	m_tAxisY.y = m_tRadius.y  * cosf( fRadOrient );
}

void TSquare::SetOrient( const TGfxVec2 & pDir )
{
	TElement::SetOrient( pDir );

	float fRadOrient = GetOrient();

	//Axis X
	m_tAxisX.x = m_tRadius.x * cosf( fRadOrient );
	m_tAxisX.y = m_tRadius.x * sinf( fRadOrient );

	//Axis Y
	m_tAxisY.x = m_tRadius.y  * -sinf( fRadOrient );
	m_tAxisY.y = m_tRadius.y  * cosf( fRadOrient );
}

const TGfxVec2 TSquare::CollideOBC(const TCircle & pCircle)const
{
	TGfxVec2 tCirclePos = pCircle.GetPos();
	//Calcul du nearest point
	const TGfxVec2 tIntersect = tCirclePos - m_tPos;
	
	//Cosinus ^ le vecteur qui relie les 2 primitive et chacun des axes de la boite
	const float fDotProductX = TGfxVec2(tIntersect).DotProduct(m_tAxisX) / m_tAxisX.SquaredLength();
	const float fDotProductY = TGfxVec2(tIntersect).DotProduct(m_tAxisY) / m_tAxisY.SquaredLength();

	TGfxVec2 tProjAxisX = m_tAxisX * fDotProductX;
	TGfxVec2 tProjAxisY = m_tAxisY * fDotProductY;

	//Clamp du cosinus
	if (tProjAxisX.Length() > m_tRadius.x)
	{
		float fLengthDif = tProjAxisX.Length() - m_tAxisX.Length();
		tProjAxisX += -tProjAxisX.Normalize() * fLengthDif;
	}

	if (tProjAxisY.Length() > m_tRadius.y)
	{
		float fLengthDif = tProjAxisY.Length() - m_tAxisY.Length();
		tProjAxisY += -tProjAxisY.Normalize() * fLengthDif;
	}
	////on multiplie le vecteur clampé par l'axe

	//on additionne les 2 vecteurs clampés et on trouve le nearest point
	TGfxVec2 tNearestPoint = m_tPos + tProjAxisX + tProjAxisY;
	
	TGfxVec2 tDistance = tCirclePos - tNearestPoint;//direction of repulsion
	float fDifference = pCircle.GetRadius() - tDistance.Length();

	if (fDifference > 0.f)
	{
		TGfxVec2 tRepulse;

		if (tDistance.x != 0.f || tDistance.y != 0.f)
		{
			tRepulse = tDistance.Normalize() * fDifference;
		}
		else//if collider position == nearest point -> repulse direction = vector AB
		{
			tRepulse = tIntersect.Normalize() * fDifference;
		}

		return tRepulse;
	}
	else
	{
		return TGfxVec2(0.f, 0.f);
	}

}

bool TSquare::CollidePoint(const TGfxVec2 & tPos)const 
{
	//Difference between colliders positions
	TGfxVec2 tDifference = m_tPos - tPos;

	//remove the sign to make it work in both direction
	tDifference = TGfxVec2(fabsf(tDifference.x), fabsf(tDifference.y));

	//remove half the radius out of the difference
	tDifference -= m_tRadius;

	//result under 0 mean that the point is colliding
	if (tDifference.x < 0.f && tDifference.y < 0.f)
		return true;

	return false;
}

TGfxVec2 TSquare::Collide(const TElement & pCollider)const
{
	const TCircle & pCircle = static_cast<const TCircle&>( pCollider );
	return CollideOBC(pCircle);
}

void TSquare::ActivateText( const char * pText, const int iDelay )
{
	m_pDelay->Start( iDelay );
	GfxTextSpritePrintf( m_pSprite, pText );
}

void TSquare::DesactivateText()
{
	if( m_pDelay != nullptr )
		m_pDelay->SetActive( false );
};

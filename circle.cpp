#include "flib.h"
#include "flib_vec2.h"
#define _INCLUDE_MATH_DEFINES
#include <math.h>

#include "circle.h"

#include "constants.h"

extern TGfxSprite * g_pSprite;

TCircle::TCircle(const TGfxVec2 & tPos, float fOrient, float fRadius)
: TElement(tPos, fOrient), m_fRadius(fRadius)
{
}

TCircle::~TCircle()
{

}

void TCircle::Update()
{
	//Move();
}

void TCircle::Render()
{
	TElement::Render();
}

TGfxVec2 TCircle::Collide(const TElement & pCollider)const
{
	const TCircle & pCircle = static_cast<const TCircle&>(pCollider);
	return CollideCC(pCircle);
}

const TGfxVec2 TCircle::CollideCC(const TCircle & pCollider)const
{
	const TGfxVec2 tColliderPos = pCollider.GetPos();
	const float fColliderRadius = pCollider.GetRadius();
	TGfxVec2 tDistance = tColliderPos - m_tPos;

	const float fRadiusSumX = (m_fRadius + fColliderRadius);
	float fDifference = m_fRadius - tDistance.Length();

	if (tDistance.Length() < fRadiusSumX)
	{
		//m_iColor = EGfxColor_Red;
		if ( (tDistance.x > 0.f || tDistance.x < 0.f) ||
			(tDistance.y > 0.f || tDistance.y < 0.f) )
			return tDistance.Normalize() * fDifference;
		else
			return TGfxVec2(0.f, 0.f);
	}
	else
	{
		//m_iColor = EGfxColor_Green;
		return TGfxVec2(0.f, 0.f);
	}
}

const TGfxVec2 TCircle::CollideCPoint(const TGfxVec2 & tPos)const
{
	TGfxVec2 tDistance = tPos - m_tPos;

	if (tDistance.Length() < m_fRadius)
	{
		//m_iColor = EGfxColor_Red;
		return tDistance;
	}
	else
	{
		//m_iColor = EGfxColor_Green;
		return TGfxVec2(0.f, 0.f);
	}
}

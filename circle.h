#pragma once

#include "element.h"
struct TGfxVec2;
struct TGfxTexture;

class TCircle : public TElement
{
public:
	TCircle(const TGfxVec2 & tPos, float fOrient, float fRadius);
	virtual ~TCircle();

	virtual void Update();
	virtual void Render();

	float GetRadius()const { return m_fRadius; };
	void SetRadius(float fRadius) { m_fRadius = fRadius; };

	virtual TGfxVec2 Collide(const TElement & pCollider)const;

	const TGfxVec2 CollideCC(const TCircle & pCollider)const;
	const TGfxVec2 CollideCPoint(const TGfxVec2 & tPos)const;
protected:
	float m_fRadius;
};

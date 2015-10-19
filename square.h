#pragma once

#include "element.h"
class TCircle;
//struct TGfxVec2;
//struct TGfxTexture;

class TSquare: public TElement
{
public:
	TSquare();//text overlay constructor
	TSquare( const TGfxVec2 & tPos );//text overlay constructor
	TSquare( const TGfxVec2 & tPos, float fOrient, const TGfxVec2 & tRadius );
	TSquare( const TGfxVec2 & tPos, float fOrient, const TGfxVec2 & tRadius, ETextureData eTexture );
	virtual ~TSquare();

	virtual void Update();
	bool UpdateText();
	virtual void Render();
	void RenderText();

	//rectangle method
	void InitAxis();
	const TGfxVec2 & GetRadius()const { return m_tRadius; };
	void SetRadius( const TGfxVec2 & tRadius ) { m_tRadius = tRadius; };
	virtual void SetOrient( const TGfxVec2 & pDir );

	virtual TGfxVec2 Collide(const TElement & pCollider)const;

	const TGfxVec2 CollideOBC(const TCircle & pCircle)const;
	bool CollidePoint(const TGfxVec2 & pPos)const;

	//text Overlay method
	void ActivateText( const char * pText, const int iDelay );
	void DesactivateText();

private:
	TGfxVec2 m_tAxisX;
	TGfxVec2 m_tAxisY;
protected:
	TGfxVec2 m_tRadius;
};


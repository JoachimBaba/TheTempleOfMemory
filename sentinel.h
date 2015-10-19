#pragma once

#include "dynamic.h"
struct TGfxTexture;

#include "constants.h"

struct TGfxVec2;
struct TGfxSprite;

class TSentinel: public TDynamic
{
public:
	TSentinel(const TGfxVec2 & tPos, float fOrient, float fRadius);
	TSentinel(const TGfxVec2 & tPos, float fOrient, float fRadius, TGfxVec2 * pDests, int iDestsCount, int iCurrentDest);
	virtual ~TSentinel();

	virtual void Repulse( const TGfxVec2 & tPos );
	
	void Move();

	void InitDests(TGfxVec2 * pDests, int iDestsCount, int iCurrentDest);


private:
	TGfxVec2 m_tDests[STEP_MAX];
	int m_iCurrentDest;
	int m_iDestCount;
};


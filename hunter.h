#pragma once

#include "dynamic.h"

enum EHunterType
{
	EHunterType_Wanderer,
	EHunterType_Chaser
};

class THunter: public TDynamic
{
public:
	THunter( int iCorner, float fRadius, float fSpeed, EHunterType eType );
	virtual ~THunter();

	virtual void Update();

	virtual void Repulse( const TGfxVec2 & tPos );
	void WandererUpdate();//random direction
	void ChaserUpdate();//follow the player
	void ResetPos( int iCorner );
	EHunterType GetType()const { return m_eType; };

protected:
	EHunterType m_eType;
};
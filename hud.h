#pragma once

#include "interface.h"
#include "item.h"

class THUD : public TInterface
{
public:
	static THUD * CreateHUD( int iCurrentRoomId );
	static void DestroyHUD();

	virtual void Render();

	void UpdateButton( int iStep, int iLevel, bool bHightLight );
	void UpdateRoomTextInfo(int iRoomId, int iScore);
	void UpdateLifePoints( unsigned char cLifePoint );

	void UpdateLights();
	
private:
	TSquare * m_pLight[LIFE_MAX + EItemType_Count];
	TSquare * m_pLifePoint[LIFE_MAX];
	static THUD * s_pInstance;
	TGfxSprite * m_pTextSprite;
	
	THUD( int iCurrentRoomId );
	virtual ~THUD();
};


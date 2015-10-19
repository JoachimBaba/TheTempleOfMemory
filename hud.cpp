#include "flib.h"
#include "flib_vec2.h"

#include "square.h"
#include "item.h"
#include "wait.h"

#include "hud.h"
#include "constants.h"

THUD * THUD::s_pInstance = nullptr;

THUD * THUD::CreateHUD( int iCurrentRoomId )
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new THUD( iCurrentRoomId );
		return s_pInstance;
	}
	return nullptr;
}

THUD::THUD( int iCurrentRoomId )
	: TInterface( HUDSettup ),
	m_pTextSprite( GfxTextSpriteCreate() )
{
	//text info
	GfxSpriteSetFilteringEnabled( m_pTextSprite, false );
	UpdateRoomTextInfo( iCurrentRoomId, 0 );

	//life points
	for( unsigned char i = 0; i < LIFE_MAX; ++i )
	{
		TGfxVec2 tPos = TGfxVec2( fLIFEPOINTS_POS_X + fLIFEPOINTS_MARGIN * i, fLIFEPOINTS_POS_Y );
		m_pLifePoint[i] = new TSquare( tPos, 0.f, TGfxVec2( fLIFEPOINTS_RADIUS, fLIFEPOINTS_RADIUS ), ETextureData_LifePoint );
	}

	//Light effect
	for( unsigned char i = 0; i < LIFE_MAX + EItemType_Count; ++i )
	{
		m_pLight[i] = nullptr;
	}
}

void THUD::DestroyHUD()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

THUD::~THUD()
{
	if( m_pTextSprite != nullptr )
		GfxSpriteDestroy( m_pTextSprite );

	for( unsigned char i = 0; i < LIFE_MAX; ++i )
	{
		delete m_pLifePoint[i];
	}
}

void THUD::Render()
{
	TInterface::Render();

	GfxTextSpriteRender(
		m_pTextSprite,
		SCREEN_SIZE_X - fHUD_ROOM_INFO_MARGIN_X - GfxSpriteGetSizeX(m_pTextSprite),
		fHUD_ROOM_INFO_MARGIN_Y,
		EGfxColor_White,
		2.f, false, false );

	for( unsigned char i = 0; i < LIFE_MAX; ++i )
	{
		m_pLifePoint[i]->Render();
	}

	for( unsigned char i = 0; i < LIFE_MAX + EItemType_Count; ++i )
	{
		if( m_pLight[i] != nullptr )
			m_pLight[i]->Render();
	}
}

void THUD::UpdateRoomTextInfo( int iRoomId, int iScore )
{
	GfxTextSpritePrintf( m_pTextSprite,
		"room\n\n  %d\n\n"
		"score\n\n  %d", iRoomId, iScore );
	/*
	if(iRoomId > 0)
		GfxTextSpritePrintf( m_pTextSprite, "room %d", iRoomId );
	else
		GfxTextSpritePrintf( m_pTextSprite, "secret\n\nroom" );
	*/
}

void THUD::UpdateLifePoints( unsigned char cLifePoint )
{
	for( unsigned char i = 0; i < LIFE_MAX; i++ )
	{
		if( i < cLifePoint )
			m_pLifePoint[i]->SetTile( 0, 0 );
		else
			m_pLifePoint[i]->SetTile( 0, 1 );
	}
}

void THUD::UpdateButton( int iStep, int iLevel, bool bHightLight )
{
	if( bHightLight && (iLevel == EButtonAnimState_Available || iLevel == EButtonAnimState_Available_Selected) )
	{
		for( unsigned char i = 0; i < EItemType_Count + LIFE_MAX; ++i )
		{
			if( m_pLight[i] == nullptr )
			{
				TGfxVec2 tPos = m_pButtons[iStep]->GetPos();
				m_pLight[i] = new TSquare( tPos, 0.f, TGfxVec2( fHUD_BUTTON_RADIUS, fHUD_BUTTON_RADIUS ), ETextureData_Light );
				m_pLight[i]->SetAnimPlayOnce( true );
				break;
			}
		}
	}

	TInterface::UpdateButton( iStep, iLevel );
}

void THUD::UpdateLights()
{
	for( unsigned char i = 0; i < EItemType_Count + LIFE_MAX; ++i )
	{
		if( m_pLight[i] != nullptr )
		{
			if( m_pLight[i]->GetAnimPlay() == false )
			{
				delete m_pLight[i];
				m_pLight[i] = nullptr;
			}
			else
			m_pLight[i]->PlayAnim();
		}
	}
}


#include "flib.h"
#include "flib_vec2.h"

#include "square.h"

#include "interface.h"
#include "constants.h"
#include "item.h"
#include "menu.h"
#include "trophy.h"

TInterface::TInterface( pSettupFunction pSettup )
{
	pSettup( m_pButtons );

}

TInterface::~TInterface()
{
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		delete m_pButtons[i];
	}
}

void TInterface::Render()
{
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if( m_pButtons[i] != nullptr )
			m_pButtons[i]->Render();
	}
}

int TInterface::CheckInput()
{
	if( GfxInputIsJustReleased( EGfxInputID_MouseLeft ) )
	{
		const float fMouseX = ( float ) GfxGetCurrentMouseX();
		const float fMouseY = ( float ) GfxGetCurrentMouseY();
		const TGfxVec2 tMousePos = TGfxVec2( fMouseX, fMouseY );

		for( int i = 0; i < BUTTON_MAX; i++ )
		{
			if( m_pButtons[i] != nullptr && m_pButtons[i]->CollidePoint( tMousePos ) )
			{
				GfxDbgPrintf( "CLICK %d\n", i );

				return i;
			}
		}
	}
	return BUTTON_NULL;
}

void TInterface::UpdateButton(int iStep, int iLevel)
{
	m_pButtons[iStep]->SetTile( iStep, iLevel );
};


void VerticalSettup( TSquare ** pButtons, int iButtonCount, const TGfxVec2 & pGroupPos, const TGfxVec2 & tButtonSize, const TGfxVec2 & tMargin, int iSelected, ETextureData eTexture )
{
	//TSquare * pButtons[BUTTON_MAX];
	
	for( int i = 0; i < BUTTON_MAX; i++ )
	{
		if( i < iButtonCount )
		{
			TGfxVec2 tButtonPos = pGroupPos +
				TGfxVec2( tButtonSize.x, tButtonSize.y * 2.f * float( i ) ) +
				( tMargin * float( i + 1 ) );

			pButtons[i] = new TSquare( tButtonPos, 0.f, tButtonSize, eTexture );

			if( i == iSelected )
				pButtons[i]->SetTile( i, EButtonAnimState_Unavailable_Selected );
			else
				pButtons[i]->SetTile( i, EButtonAnimState_Unavailable );
		}
		else
		{
			pButtons[i] = nullptr;
		}
	}

	//return &pButtons[0];
}

void HUDSettup( TSquare ** pButtons )
{
	TGfxVec2 tGroupPos = TGfxVec2( fHUD_BUTTON_POS_X, fHUD_BUTTON_POS_Y );
	TGfxVec2 tButtonSize = TGfxVec2( fHUD_BUTTON_RADIUS, fHUD_BUTTON_RADIUS );
	TGfxVec2 tMargin = TGfxVec2( 0.f, fHUD_BUTTON_MARGIN );

	VerticalSettup(pButtons, EItemType_Count, tGroupPos, tButtonSize, tMargin, 0, ETextureData_HUD );

	//return &pButtons[0];
}

void MainMenuSettup( TSquare ** pButtons )
{
	TGfxVec2 tGroupPos = TGfxVec2( fMENU_BUTTON_POS_X, fMENU_BUTTON_POS_Y );
	TGfxVec2 tButtonSize = TGfxVec2( fMENU_BUTTON_RADIUS_X, fMENU_BUTTON_RADIUS_Y );
	TGfxVec2 tMargin = TGfxVec2( 0.f, fMENU_BUTTON_MARGIN );

	VerticalSettup(pButtons, EMainMenuState_Count, tGroupPos, tButtonSize, tMargin, BUTTON_NULL, ETextureData_MainMenu );

	//return &pButtons[0];
}

void MenuPageSettup( TSquare ** pButtons )
{
//	TSquare * pButtons[BUTTON_MAX];
	
	TTileInfo tTileSize = TTextureControler::GetTileSize( ETextureData_MenuButton );
	TGfxVec2 tButtonSize = TGfxVec2( float( tTileSize.x/2 ), float( tTileSize.y /2) );
	TGfxVec2 tPos = TGfxVec2( ( float ) SCREEN_SIZE_X, ( float ) SCREEN_SIZE_Y ) - tButtonSize;

	pButtons[0] = new TSquare( tPos, 0.f, tButtonSize, ETextureData_MenuButton );

	for( int i = 1; i < BUTTON_MAX; i++ )
	{
		pButtons[i] = nullptr;
	}

	//return &pButtons[0];
}

void TrophySettup( TSquare ** pButtons )
{
	//TSquare * pButtons[BUTTON_MAX];

	TGfxVec2 tScreenCenter = TGfxVec2( ( float ) SCREEN_SIZE_X/2, ( float ) SCREEN_SIZE_Y/2 );
	TTileInfo tTileSize = TTextureControler::GetTileSize( ETextureData_Trophy );
	TGfxVec2 tButtonSize = TGfxVec2( float( tTileSize.x ), float( tTileSize.y ) );

	TGfxVec2 tMarginX = TGfxVec2( fTROPHY_MARGIN, 0.f );
	TGfxVec2 tMarginY = TGfxVec2( 0.f, fTROPHY_MARGIN );
	int iColCount = ( TROPHY_MAX / 2 );

	TGfxVec2 tGroupPos = tScreenCenter - ( ( TGfxVec2( tButtonSize.x, tButtonSize.y/2 ) + tMarginX ) * float( iColCount / 2 ) );

	for( int i = 0; i < BUTTON_MAX; i++ )
	{
		TGfxVec2 tPosX = ( TGfxVec2( tButtonSize.x, 0.f) + tMarginX ) * float( i % iColCount );
		TGfxVec2 tPosY = ( TGfxVec2( 0.f, tButtonSize.y ) + tMarginY) * float( i / iColCount );
		TGfxVec2 tButtonPos = tGroupPos + tPosX + tPosY;

		pButtons[i] = new TSquare( tButtonPos, 0.f, tButtonSize/2, ETextureData_Trophy );

		pButtons[i]->SetTile( i, EButtonAnimState_Unavailable );
	}
	
	//return &pButtons[0];
}





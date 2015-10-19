#include "flib.h"

#include "interface.h"
#include "player.h"
#include "door.h"
#include "item.h"
#include "menu.h"
#include "trophy.h"

#include "texture_controler.h"
#include "constants.h"

TTextureControler::TTextureControler()
{
	for( int i = 0; i < ETextureData_Count; ++i )
	{
		switch( i )
		{
		case ETextureData_Player:
		case ETextureData_Sentinel:
		case ETextureData_Chaser:
		case ETextureData_Wanderer:
		{
			m_pTexture[i] = nullptr;
			if( i == ETextureData_Player )
				m_pTexture[i] = GfxTextureLoad( "Player_TileSet.tga" );
			else if( i == ETextureData_Sentinel )
				m_pTexture[i] = GfxTextureLoad( "Sentinel_TileSet.tga" );
			else if( i == ETextureData_Chaser )
				m_pTexture[i] = GfxTextureLoad( "Chaser_TileSet.tga" );
			else if( i == ETextureData_Wanderer )
				m_pTexture[i] = GfxTextureLoad( "Wanderer_TileSet.tga" );

			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] ) / EDynamicAnimState_Count;
			int iSizeX = iSizeY;

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );

		}
			break;

		case ETextureData_HUD:
		{
			m_pTexture[i] = GfxTextureLoad( "Button_TileSet.tga" );

			int iSizeX = GfxTextureGetSizeX( m_pTexture[i] ) / EButtonAnimState_Count;
			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] ) / EItemType_Count;

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );

		}
			break;

		case ETextureData_MainMenu:
		{
			m_pTexture[i] = GfxTextureLoad( "mainMenu_Button.tga" );

			int iSizeX = GfxTextureGetSizeX( m_pTexture[i] ) / EMainMenuState_Count;
			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] );

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );

		}
			break;

		case ETextureData_Door:
		{
			m_pTexture[i] = GfxTextureLoad( "Door_TileSet.tga" );

			int iSizeX = GfxTextureGetSizeX( m_pTexture[i] ) / 4;
			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] ) / EDoor_State_Count;

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );

		}
			break;

		case ETextureData_Item:
		{
			m_pTexture[i] = GfxTextureLoad( "Item_TileSet.tga" );

			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] ) / EItemType_Count;
			int iSizeX = iSizeY;

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );

		}
			break;

		case ETextureData_LifePoint:
		{
			m_pTexture[i] = GfxTextureLoad( "LifePoints_TileSet.tga" );

			int iSizeX = GfxTextureGetSizeX( m_pTexture[i] );
			int iSizeY = iSizeX;

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );

		}
			break;

		case ETextureData_MenuButton:
		{
			m_pTexture[i] = GfxTextureLoad( "menuButton.tga" );

			int iSizeX = GfxTextureGetSizeX( m_pTexture[i] );
			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] );

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );
		}
			break;

		case ETextureData_Trophy:
		{
			m_pTexture[i] = GfxTextureLoad( "Trophies.tga" );

			int iSizeX = GfxTextureGetSizeX( m_pTexture[i] ) / TROPHY_MAX;
			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] ) / 2;

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );
		}
			break;

		case ETextureData_Light:
		{
			m_pTexture[i] = GfxTextureLoad( "LightEffect.tga" );

			int iSizeY = GfxTextureGetSizeY( m_pTexture[i] );
			int iSizeX = iSizeY;// GfxTextureGetSizeX( m_pTexture[i] ) / iSizeY;

			m_tTileSize[i] = TTileInfo( iSizeX, iSizeY );
		}
			break;

		default:
			m_pTexture[i] = nullptr;
			m_tTileSize[i] = TTileInfo( 0, 0 );
			break;
		}
	}
}

int TTextureControler::GetTileCountX(ETextureData eTexture)
{
	int iTextureSizeX = GfxTextureGetSizeX( s_pInstance->m_pTexture[ eTexture ] );
	int iTileSizeX = s_pInstance->m_tTileSize[eTexture].x;
	return iTextureSizeX / iTileSizeX;
}

int TTextureControler::GetTileCountY( ETextureData eTexture )
{
	int iTextureSizeY = GfxTextureGetSizeY( s_pInstance->m_pTexture[eTexture] );
	int iTileSizeY = s_pInstance->m_tTileSize[eTexture].y;
	return iTextureSizeY / iTileSizeY;
}

TTextureControler::~TTextureControler()
{
	for( int i = 0; i < ETextureData_Count; ++i )
	{
		if( m_pTexture[i] != nullptr )
			GfxTextureDestroy( m_pTexture[i] );
	}
}


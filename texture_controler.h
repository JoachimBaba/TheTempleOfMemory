#pragma once
#include "tileinfo.h"
#include "singleton.h"

struct TGfxTexture;

enum ETextureData
{
	ETextureData_Player,
	ETextureData_HUD,
	ETextureData_MainMenu,
	ETextureData_Sentinel,
	ETextureData_Chaser,
	ETextureData_Wanderer,
	ETextureData_Door,
	ETextureData_Item,
	ETextureData_LifePoint,
	ETextureData_MenuButton,
	ETextureData_Trophy,
	ETextureData_Light,

	ETextureData_Count
};

struct TTextureControler : public TSingleton<TTextureControler>
{
	friend class TSingleton < TTextureControler > ;
private:

	TGfxTexture * m_pTexture[ETextureData_Count];
	TTileInfo m_tTileSize[ETextureData_Count];

	TTextureControler();
	~TTextureControler();

public:

	static TGfxTexture * GetTexture( ETextureData eTexture ) { return s_pInstance->m_pTexture[eTexture]; };
	static const TTileInfo & GetTileSize( ETextureData eTexture ) { return s_pInstance->m_tTileSize[eTexture]; };
	static int GetTileCountX( ETextureData eTexture );
	static int GetTileCountY( ETextureData eTexture );
};

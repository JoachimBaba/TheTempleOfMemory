#include "flib.h"
#include "flib_vec2.h"

//#include "constants.h"

#include "bar.h"

TBar::TBar()
	: m_tPos(TGfxVec2(0.f, 0.f)), m_tRadius(TGfxVec2(1.f, 1.f)), m_fLevel(0), m_iColor(EGfxColor_White), m_pSprite(nullptr), m_pTexture(nullptr)
{

}

TBar::TBar( const TGfxVec2 & tPos, const TGfxVec2 & tRadius, unsigned int iColor, float fLevel )
	: m_tPos(tPos), m_tRadius(tRadius), m_fLevel(fLevel), m_iColor(iColor), m_pSprite(nullptr), m_pTexture(nullptr)
{
	TGfxImage * pImage = GfxImageCreate(1, 1);
	unsigned int * pData = GfxImageGetData( pImage );
	pData[0] = m_iColor;
	m_pTexture = GfxTextureCreate( pImage );
	m_pSprite = GfxSpriteCreate( m_pTexture );

	GfxImageDestroy( pImage );
	pData = nullptr;

	GfxSpriteSetPosition( m_pSprite, m_tPos.x, m_tPos.y );
	GfxSpriteSetScale(m_pSprite, 1.f, m_tRadius.y);
}

TBar::~TBar()
{
	if (m_pSprite != nullptr)
	{
		GfxSpriteDestroy(m_pSprite);
		m_pSprite = nullptr;
	}
	if( m_pTexture != nullptr )
	{
		GfxTextureDestroy( m_pTexture );
		m_pTexture = nullptr;
	}
}

void TBar::Update()
{
	Draw();
}

void TBar::Render()
{
	if( m_pSprite != nullptr )
		GfxSpriteRender( m_pSprite );
}

void TBar::Draw()
{
	float fPart = m_tRadius.x * m_fLevel;
	GfxSpriteSetScale( m_pSprite, fPart, m_tRadius.y );
}


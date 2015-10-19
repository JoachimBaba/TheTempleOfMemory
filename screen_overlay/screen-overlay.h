#ifndef SCREEN_OVERLAY_H
#define SCREEN_OVERLAY_H

#include "screen-overlay-data.h"

struct TGfxImage;
struct TGfxTexture;
struct TGfxSprite;

class TScreenOverlay
{
public:
	TScreenOverlay( EScreenOverlayState eState, float fDelay );
	~TScreenOverlay();
	EScreenOverlayState Update();
	void Start( EScreenOverlayState eState );

	//Getter
	TGfxImage * GetImage()const { return m_pImage; };
	TGfxTexture * GetTexture()const { return m_pTexture; };
	TGfxSprite * GetSprite()const { return m_pSprite; };
	unsigned int * GetImageData()const { return m_iImageData; };
	TScreenOverlayData GetData()const { return m_tData; };
	EScreenOverlayState GetState()const {
		//GfxDbgPrintf( "GetState %d\n", m_tData.m_eState );
		return m_tData.m_eState;
	};

	void Render();


private:

	TGfxImage * m_pImage;
	unsigned int * m_iImageData;
	TGfxTexture * m_pTexture;
	TGfxSprite * m_pSprite;

	TScreenOverlayData m_tData;

	void FadeIn();
	void FadeOut();

	//Setter
	void UpdateData( float fDelay, int iAlphaMax, bool bSwitchedInCourse, EScreenOverlayState eState )
	{
		m_tData.m_fDelay = fDelay;
		m_tData.m_iAlphaMax = iAlphaMax;
		m_tData.m_bSwitchedInCourse = bSwitchedInCourse;
		m_tData.m_eState = eState;
		SetColor(iAlphaMax);
	}
	void SetColor( int iAlphaLevel )
	{
		m_tData.m_iAlphaLevel = iAlphaLevel;
		//GfxDbgPrintf( "%d\n", iAlphaLevel );
		GfxSpriteSetColor( m_pSprite, GfxColor( 0, 0, 0, iAlphaLevel ) );
	}
	unsigned int * InitImageData( TGfxImage * pImage )
	{
		m_iImageData = GfxImageGetData( pImage );
		m_iImageData[0] = EGfxColor_White;

		return m_iImageData;
	}
};

#endif // !SCREEN_OVERLAY_H

#include "flib.h"
#include "screen-overlay.h"

TScreenOverlay::TScreenOverlay( EScreenOverlayState eState, float fDelay )
:	m_pImage( GfxImageCreate(1, 1) ),
	m_iImageData( InitImageData(m_pImage) ),
	m_pTexture( GfxTextureCreate(m_pImage) ),
	m_pSprite( GfxSpriteCreate( m_pTexture, 0, 0, GfxGetDisplaySizeX(), GfxGetDisplaySizeY() ) ),
	m_tData(eState, fDelay)
{
	//Set Default Color
	GfxSpriteSetColor( m_pSprite, GfxColor(0, 0, 0, m_tData.m_iAlphaLevel) );
}

TScreenOverlay::~TScreenOverlay()
{
	if( m_pSprite != nullptr )
		GfxSpriteDestroy( m_pSprite );
	if( m_pTexture != nullptr )
		GfxTextureDestroy( m_pTexture );
	if( m_pImage != nullptr )
		GfxImageDestroy( m_pImage );
}

void TScreenOverlay::FadeIn()
{
	float fAlphaValue;
	int iAlphaLevel = m_tData.m_iAlphaLevel;

	const int iCurrentTime = GfxTimeGetMilliseconds();
	const int iStateTime = m_tData.m_iStateTime;
	const float fTimeProgress = float(iCurrentTime - iStateTime);
	const float fTimePercent = fTimeProgress / m_tData.m_fDelay;

	if (m_tData.m_bSwitchedInCourse == false)
	{
		fAlphaValue = OPAQUE * (fTimePercent);
		iAlphaLevel = OPAQUE - int(fAlphaValue);
	}
	else
	{
		fAlphaValue = m_tData.m_iAlphaMax * (fTimePercent);
		iAlphaLevel = m_tData.m_iAlphaMax - int(fAlphaValue);
	}

	if( iAlphaLevel < TRANSPARENT || iAlphaLevel > OPAQUE )
	{
		const float fNewDelay = m_tData.GetMaxDelay();
		iAlphaLevel = TRANSPARENT;
		UpdateData( fNewDelay, OPAQUE, false, EScreenOverlayState_AllTranslucent );

		//GfxDbgPrintf( "fixed error : Faded In value below TRANSPARENT (%d)\n", TRANSPARENT );
	}

	SetColor(iAlphaLevel);

}

void TScreenOverlay::FadeOut()
{
	float fAlphaValue;
	int iAlphaLevel = m_tData.m_iAlphaLevel;

	const int iCurrentTime = GfxTimeGetMilliseconds();
	const int iStateTime = m_tData.m_iStateTime;
	const float fTimeProgress = float(iCurrentTime - iStateTime);
	const float fTimePercent = fTimeProgress / m_tData.m_fDelay;
	
	if (m_tData.m_bSwitchedInCourse == false)
	{
		fAlphaValue = OPAQUE * (fTimePercent);
		iAlphaLevel = int(fAlphaValue);
	}
	else
	{
		fAlphaValue = m_tData.m_iAlphaMax * (fTimePercent);
		iAlphaLevel = m_tData.m_iAlphaMax + int(fAlphaValue);
	}

	if( iAlphaLevel > OPAQUE || iAlphaLevel < TRANSPARENT )
	{
		const float fNewDelay = m_tData.GetMaxDelay();
		iAlphaLevel = OPAQUE;
		UpdateData( fNewDelay, OPAQUE, false, EScreenOverlayState_AllBlack );

		//GfxDbgPrintf( "fixed error : FadeOut value over OPAQUE (%d)\n", OPAQUE );
	}

	SetColor(iAlphaLevel);

}

void TScreenOverlay::Start( EScreenOverlayState eState )
{
	if( eState == EScreenOverlayState_FadingIn )
	{
		m_tData.m_eState = EScreenOverlayState_FadingIn;
		//GfxDbgPrintf( "Fading in\n" );
			
		int iCurrenTime = GfxTimeGetMilliseconds();

		if (m_tData.m_iAlphaLevel != OPAQUE)
		{
			/*
			const float fNewDelay = m_tData.m_fDelay - ( iCurrenTime - m_tData.m_iStateTime );
			const int iNewAlphaMax = m_tData.m_iAlphaLevel;

			UpdateData( fNewDelay, iNewAlphaMax, true, EScreenOverlayState_FadingIn );
			*/
			SetColor(OPAQUE);
		}
		else
		{
			const float fNewDelay = m_tData.GetMaxDelay();
			UpdateData( fNewDelay, OPAQUE, false, EScreenOverlayState_FadingIn );
		}
		m_tData.m_iStateTime = iCurrenTime;			

		//GfxDbgPrintf("%f\n", static_cast<float>( iCurrenTime/1000) );
	}
	else if( eState == EScreenOverlayState_FadingOut )
	{
		m_tData.m_eState = EScreenOverlayState_FadingOut;
		//GfxDbgPrintf( "Fading out\n" );

		const int iCurrenTime = GfxTimeGetMilliseconds();

		if( m_tData.m_iAlphaLevel != TRANSPARENT )
		{
			/*
			const float fNewDelay = float( iCurrenTime - m_tData.m_iStateTime );
			const int iNewAlphaMax = m_tData.m_iAlphaLevel;

			UpdateData( fNewDelay, iNewAlphaMax, true, EScreenOverlayState_FadingOut );
			*/
			SetColor(TRANSPARENT);
		}
		else
		{
			const float fNewDelay = m_tData.GetMaxDelay();
			UpdateData( fNewDelay, TRANSPARENT, false, EScreenOverlayState_FadingOut );
		}
			
		m_tData.m_iStateTime = iCurrenTime;

		//GfxDbgPrintf( "%f\n", static_cast<float>( iCurrenTime / 1000 ) );

	}
	else if( eState == EScreenOverlayState_AllBlack )
	{
		m_tData.m_eState = EScreenOverlayState_AllBlack;
	}
	else if( eState == EScreenOverlayState_AllTranslucent )
	{
		m_tData.m_eState = EScreenOverlayState_AllTranslucent;
	}
}

EScreenOverlayState TScreenOverlay::Update()
{
	switch( m_tData.m_eState )
	{
		case EScreenOverlayState_AllBlack:
			m_tData.m_iAlphaMax = OPAQUE;

			if (m_tData.m_iAlphaLevel != OPAQUE)
			SetColor(OPAQUE);
			break;

		case EScreenOverlayState_FadingIn:
			FadeIn();
			break;

		case EScreenOverlayState_AllTranslucent:
			m_tData.m_iAlphaMax = OPAQUE;

			if (m_tData.m_iAlphaLevel != TRANSPARENT)
			SetColor(TRANSPARENT);
			break;

		case EScreenOverlayState_FadingOut:
			FadeOut();
			break;
	}
	return m_tData.m_eState;
}


void TScreenOverlay::Render()
{
	GfxSpriteRender( m_pSprite );
}


#include "flib.h"
#include "screen-overlay-data.h"

TScreenOverlayData::TScreenOverlayData()
	: m_fDelay( 0.f ), m_fMaxDelay( 0.f ), m_eState( EScreenOverlayState_AllTranslucent ), m_iStateTime( 0 ), m_iAlphaLevel( 255 ), m_iAlphaMax( 0 ), m_bSwitchedInCourse( false )
{

}

TScreenOverlayData::TScreenOverlayData(EScreenOverlayState m_eState, float fDelay)
	: m_fDelay( fDelay ), m_fMaxDelay( fDelay ), m_eState( m_eState ), m_iStateTime( 0 ), m_iAlphaMax( 0 ), m_bSwitchedInCourse( false )
{
	//Alpha level depends of the state
	switch (m_eState)
	{
	case EScreenOverlayState_AllBlack:
		m_iAlphaLevel = OPAQUE;
		break;
	case EScreenOverlayState_AllTranslucent:
		m_iAlphaLevel = TRANSPARENT;
		break;
	case EScreenOverlayState_FadingOut:
		m_iAlphaLevel = OPAQUE;
		break;
	case EScreenOverlayState_FadingIn:
		m_iAlphaLevel = TRANSPARENT;
		break;
	default:
		GfxDbgAssert(false, "m_eState is undefined\n");
		break;
	}
}

TScreenOverlayData::~TScreenOverlayData()
{

}

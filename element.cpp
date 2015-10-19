#define _USE_MATH_DEFINES
#include <math.h>

#include "tileinfo.h"
#include "texture_controler.h"

#include "element.h"
#include "constants.h"
#include "wait.h"

TElement::TElement( const TGfxVec2 & tPos, float fOrient )
	: m_tAnimInfo( fANIM_DELAY_MAX, 0, 0, true, false ),
	m_tPos( tPos ),
	m_pDelay( nullptr ),
	m_pSprite( nullptr ),
	m_fOrient( GfxMathDegToRad( fOrient ) )
{
}

TElement::~TElement()
{
	if( m_pSprite != nullptr )
		GfxSpriteDestroy( m_pSprite );

	delete m_pDelay;
}

void TElement::Render()//unused -> all instance share the same sprite
{
	if( m_pSprite != nullptr )
		GfxSpriteRender( m_pSprite );
}

void TElement::SetOrient( const TGfxVec2 & pDir )
{
	m_fOrient = atan2f( pDir.y, pDir.x );

	//GfxDbgPrintf("%f\n", GfxMathRadToDeg(m_fOrient));dbg

	GfxSpriteSetAngle( m_pSprite, -m_fOrient );
}

void TElement::PlayAnim()
{
	if (m_tAnimInfo.m_bPlaying)
	{
		if( m_tAnimInfo.m_bPlayOnce && IsLastStep() )
			m_tAnimInfo.m_bPlaying = false;
		else
		{
			m_tAnimInfo.m_fInterval += GfxTimeFrameGetCurrentDurationInSeconds();

			if( m_tAnimInfo.m_fInterval >= fANIM_DELAY_MAX )
			{
				m_tAnimInfo.m_fInterval = 0.f;

				if(m_tAnimInfo.m_bReverse)
					m_tAnimInfo.m_iStep--;
				else
					m_tAnimInfo.m_iStep++;

				TTileInfo tSpriteSize = TTileInfo(GfxSpriteGetSizeX(m_pSprite), GfxSpriteGetSizeY(m_pSprite));
				TTileInfo tTileCount = TTileInfo(GfxSpriteGetTextureSizeX(m_pSprite) / tSpriteSize.x, GfxSpriteGetTextureSizeY(m_pSprite) / tSpriteSize.y);

				if( m_tAnimInfo.m_iStep >= tTileCount.x )
					m_tAnimInfo.m_iStep = 0;
				else if( m_tAnimInfo.m_iStep < 0 )
					m_tAnimInfo.m_iStep = tTileCount.x - 1;

				int iPosX = m_tAnimInfo.m_iStep * tSpriteSize.x;
				int iPosY = m_tAnimInfo.m_iLevel * tSpriteSize.y;
		
				GfxSpriteSetCutout(m_pSprite, iPosX, iPosY, tSpriteSize.x, tSpriteSize.y);
			}
		}
	}
}
void TElement::SetAnimLevel( const int iLevel )
{
	m_tAnimInfo.m_iLevel = iLevel;
	m_tAnimInfo.m_fInterval = 0.f;

	if(m_tAnimInfo.m_bReverse == false)
		m_tAnimInfo.m_iStep = 0;
	else
	{
		int iLastStep = ( GfxSpriteGetTextureSizeX( m_pSprite ) / GfxSpriteGetSizeX( m_pSprite ) ) - 1;
		m_tAnimInfo.m_iStep = iLastStep;
	}
}
void TElement::StartAnim( const int iLevel, const bool bPlayOnce )
{
	SetAnimLevel( iLevel );
	SetAnimPlayOnce( bPlayOnce );
}

void TElement::SetTile(const int iStep, const int iLevel)
{
	TTileInfo tSpriteSize = TTileInfo(GfxSpriteGetSizeX(m_pSprite), GfxSpriteGetSizeY(m_pSprite));

	m_tAnimInfo.m_iStep = iStep;
	m_tAnimInfo.m_iLevel = iLevel;

	int iPosX = m_tAnimInfo.m_iStep * (int)tSpriteSize.x;
	int iPosY = m_tAnimInfo.m_iLevel * (int)tSpriteSize.y;

	GfxSpriteSetCutout(m_pSprite, iPosX, iPosY, tSpriteSize.x, tSpriteSize.y);
}

void TElement::SpriteSettup(ETextureData eTexture, const TGfxVec2 & tObjectSize)
{
	TGfxTexture * pTexture = TTextureControler::GetTexture(eTexture);
	TTileInfo tSpriteSize = TTextureControler::GetTileSize(eTexture);

	int iPosX = m_tAnimInfo.m_iStep * (int)tObjectSize.x;
	int iPosY = m_tAnimInfo.m_iLevel * (int)tObjectSize.y;

	//sprite creation
	m_pSprite = GfxSpriteCreate( pTexture, iPosX, iPosY, tSpriteSize.x, tSpriteSize.y );
	GfxSpriteSetFilteringEnabled(m_pSprite, false);

	//position
	GfxSpriteSetPosition(m_pSprite, m_tPos.x, m_tPos.y);

	//scale
	float fScaleX = tObjectSize.x / tSpriteSize.x;
	float fScaleY = tObjectSize.y / tSpriteSize.y;
	GfxSpriteSetScale(m_pSprite, fScaleX*2, fScaleY*2);
	
	//pivot
	GfxSpriteSetPivot( m_pSprite, tSpriteSize.x / 2.f, tSpriteSize.y / 2.f );

	//orient
	GfxSpriteSetAngle(m_pSprite, m_fOrient);
}

bool TElement::IsWaiting()const
{
	if( m_pDelay != nullptr )
		return m_pDelay->IsActive();
	else
		return false;
}

bool TElement::SetDelay( const int iDelay, const bool bActive )
{
	int iTime = GfxTimeGetMilliseconds();

	if( m_pDelay == nullptr )
	{
		m_pDelay = new TWait( iTime, iDelay, bActive );
		return true;
	}
	else
	{
		m_pDelay->SetStartTime( iTime );
		m_pDelay->SetDelayTime( iDelay );
		m_pDelay->SetActive( bActive );
		return false;
	}
}

bool TElement::IsLastStep()const
{
	if( m_tAnimInfo.m_bReverse && m_tAnimInfo.m_iStep == 0 )
		return true;
	else
	{
		int iAnimStep = GetAnimStep();
		int iLastStep = GetLastAnimStep();

		if( iAnimStep == iLastStep )
			return true;
	}
	return false;
}


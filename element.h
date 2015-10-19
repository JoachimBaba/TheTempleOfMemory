#pragma once

#include "flib.h"
#include "flib_vec2.h"
#include "texture_controler.h"

struct TGfxSprite;
struct TGfxTexture;
struct TGfxVec2;
struct TTextureControler;
struct TWait;

class TElement
{
public:
	TElement(const TGfxVec2 & tPos, float fOrient);
	virtual ~TElement();

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual TGfxVec2 Collide(const TElement & pCollider)const = 0;

	//Getter
	const TGfxVec2 & GetPos()const { return m_tPos; };
	float GetOrient()const { return m_fOrient; };
	TWait * GetDelay()const { return m_pDelay; };
	TGfxSprite * GetSprite() { return m_pSprite; };
	bool IsWaiting()const;


	//Setter
	void SetPos( const TGfxVec2 & tPos ) { m_tPos = tPos; GfxSpriteSetPosition( m_pSprite, m_tPos.x, m_tPos.y ); };
	void SetOrient( const float fOrient ) { m_fOrient = fOrient; GfxSpriteSetAngle( m_pSprite, -m_fOrient );};
	virtual void SetOrient( const TGfxVec2 & pDir );
	bool SetDelay( const int iDelay, const bool bActive );
	
	//Animation
	void PlayAnim();
	void SetTile(const int iStep, const int iLevel);

	void SetAnimPlay(const bool bPlaying) { m_tAnimInfo.m_bPlaying = bPlaying; };
	void SetAnimLevel( const int iLevel );
	void SetAnimReverse( const bool bReverse ) { m_tAnimInfo.m_bReverse = bReverse; };
	void SetAnimPlayOnce( const bool bPlayOnce ) {m_tAnimInfo.m_bPlayOnce = bPlayOnce; };
	void StartAnim( const int iLevel, const bool bPlayOnce );

	int GetAnimLevel()const { return m_tAnimInfo.m_iLevel; };
	int GetAnimStep()const { return m_tAnimInfo.m_iStep; };
	bool GetAnimReverse()const { return m_tAnimInfo.m_bReverse; };
	bool GetAnimPlay()const { return m_tAnimInfo.m_bPlaying; };
	int GetLastAnimStep()const { return ( GfxSpriteGetTextureSizeX( m_pSprite ) / GfxSpriteGetSizeX( m_pSprite ) ) - 1; };

	bool IsLastStep()const;
	//void FlashTransition( const int iDuration, const int iDelay, const int FromLevel, const int ToLevel );

	struct TAnimInfo
	{
		friend class TElement;
	private:
		float m_fInterval;

		int m_iStep;
		int m_iLevel;

		bool m_bPlaying;
		bool m_bReverse;
		bool m_bPlayOnce;
	public:
		TAnimInfo(): m_fInterval( 0.f ), m_iStep( 0 ), m_iLevel( 0 ), m_bPlaying( false ), m_bReverse( false ), m_bPlayOnce( false ){};
		TAnimInfo( float fInterval, int iStep, int iLevel, bool bPlaying, bool bPlayOnce )
			: m_fInterval( fInterval ),
			m_iStep( iStep ),
			m_iLevel( iLevel ),
			m_bPlaying( bPlaying ),
			m_bReverse( false ),
			m_bPlayOnce( bPlayOnce ){};
	};

protected:
	void SpriteSettup(ETextureData eTexture, const TGfxVec2 & tObjectSize);

	TAnimInfo m_tAnimInfo;
	TGfxVec2 m_tPos;
	TWait * m_pDelay;
	TGfxSprite * m_pSprite;

private:
	float m_fOrient;

};



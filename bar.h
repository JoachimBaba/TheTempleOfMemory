#pragma once

struct TGfxSprite;
struct TGfxTexture;
struct TGfxVec2;

class TBar
{
public:
	TBar();
	TBar(const TGfxVec2 & tPos, const TGfxVec2 & tRadius, unsigned int iColor, float fLevel);
	~TBar();
	
	void Update();
	void Render();

	void Draw();

	const TGfxVec2 & GetPos()const { return m_tPos; };
	const TGfxVec2 & GetRadius()const { return m_tRadius; };
	unsigned int GetColor()const { return m_iColor; };
	float GetLevel()const { return m_fLevel; };

	void SetPos( const TGfxVec2 & tPos ) { m_tPos = tPos; };
	void SetRadius( const TGfxVec2 & tRadius ) { m_tRadius = tRadius; };
	void SetColor( const unsigned int iColor ) { m_iColor = iColor; };
	void SetLevel( const float fLevel ) { m_fLevel = fLevel; };

private:
	TGfxVec2 m_tPos;
	TGfxVec2 m_tRadius;
	float m_fLevel;
	unsigned int m_iColor;
	TGfxSprite * m_pSprite;
	TGfxTexture * m_pTexture;
};


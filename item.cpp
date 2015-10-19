#include "flib.h"
#include "flib_vec2.h"

#include "player.h"

#include "item.h"

#include "constants.h"

TItem::TItem(const TGfxVec2 & tPos, float fRadius, EItemType eType)
	:TCircle( tPos, 0.f, fRadius ), m_eType( eType )
{
	//Sprite settup
	SpriteSettup(ETextureData_Item, TGfxVec2(m_fRadius, m_fRadius));
	SetTile(0, eType);
}

TItem::~TItem()
{

}



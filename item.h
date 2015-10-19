#pragma once

#include "circle.h"

struct TWait;

enum EItemType
{
	EItemType_Empty,
	EItemType_Sword,
	EItemType_Locker,
	EItemType_Key,

	EItemType_Count,
	EItemType_Null,
};

class TItem: public TCircle
{
public:
	TItem( const TGfxVec2 & tPos, float fRadius, EItemType eType );
	virtual ~TItem();
	
	EItemType GetType()const { return m_eType; };

private:
	EItemType m_eType;
};
#pragma once

#include "dynamic.h"
#include "item.h"

struct TGfxVec2;
struct TGfxSprite;
struct TGfxTexture;
class TRoom;
class TDoor;
class THUD;

class TPlayer : public TDynamic
{
public:
	TPlayer(const TGfxVec2 & tPos, float fOrient, float fRadius, TRoom * pCurrentRoom);
	virtual ~TPlayer();
	
	virtual void Update();
	virtual void Render();
	
	virtual void Move();
	virtual void Repulse( const TGfxVec2 & tPos );
		
	const TGfxVec2 & GetDest()const { return m_tDest; };
	void SetDest(const TGfxVec2 & tDest) { m_tDest = tDest; };
	int GetButton();
	
	//doors & rooms
	bool EnterRoom( TDoor const * tDoor);
	TRoom & GetCurrentRoom()const { return *m_pCurrentRoom; };
	void SetToDoorPos(const TDoor & pDoor);

	//inventory
	EItemType YieldActualItem();
	bool CheckInventory(const EItemType eItem) { return m_bInventory[eItem]; };
	bool CheckSelectedAndCollected( const EItemType eItem )
	{
		if(m_eSelectedItem == eItem && m_bInventory[m_eSelectedItem] == true)
			return true;
		return false;
	};
	EItemType GetSelectedItem() { return m_eSelectedItem; };
	bool CollectItem( const EItemType eItem );

	int GetScore()const { return m_iScore; };

	void LooseLife();
	void Hurt();
	bool StillAlive()const;

private:

	TItem * m_pConsumedItem[EItemType_Count - 1];
	bool m_bInventory[EItemType_Count];
	TGfxVec2 m_tDest;
	EItemType m_eSelectedItem;
	TRoom * m_pCurrentRoom;
	THUD * m_pHUD;
	int m_iScore;
	unsigned char m_cLifePoint;

	void GetInput();
	virtual void Die();
	void UpdateConsumedItemFeedBack();
};


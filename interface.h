#pragma once

struct TGfxVec2;
class TSquare;

#include "texture_controler.h"

#include "constants.h"

enum EButtonAnimState
{
	EButtonAnimState_Unavailable,
	EButtonAnimState_Available,
	EButtonAnimState_Unavailable_Selected,
	EButtonAnimState_Available_Selected,

	EButtonAnimState_Count
};

typedef void ( *pSettupFunction )( TSquare ** pButtons );

void HUDSettup(TSquare ** pButtons);
void MainMenuSettup(TSquare ** pButtons);
void MenuPageSettup(TSquare ** pButtons);
void TrophySettup(TSquare ** pButtons);

class TInterface
{
public:
	TInterface( pSettupFunction pSettup );
	virtual ~TInterface();

	virtual void Render();

	int CheckInput();
	void UpdateButton( int iStep, int iLevel );

protected:
	TSquare * m_pButtons[BUTTON_MAX];

};

#include "flib.h"
#include "flib_vec2.h"

#include "controler.h"
#include "texture_controler.h"
#include "element.h"
#include "game.h"

#include "constants.h"

TControler * g_pControler;

void Initialize()
{
	//seed initialization
	const unsigned int iSeed = GfxMathGetRandomInteger( 0, 65535 ); //35153;//12798//18825
	GfxMathResetRandomSeed( iSeed );

	GfxDbgPrintf( "SEED : %d", iSeed );

	//entity manager initialization 
	g_pControler = TControler::CreateInstance();
}

void Update()
{
	g_pControler->Update();
}

void Render()
{
	GfxClear(EGfxColor_Black);

	if (g_pControler != nullptr)
		g_pControler->Render();
}

void Finalize()
{
	if (g_pControler != nullptr)
	{
		g_pControler->DestroyInstance();
	}
}

void GfxMain(int, char *[])
{
	GfxDefaultResolution(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	GfxCallbacks( Initialize, Update, Render, Finalize );
}
/*
//TODO
graphics:
	- anim plus simple (zelda like)
	- repers visuel:
		- les portes de la premi!�re salle ont un skin bien sp�cifique

		- indiquer explicitement quand un item est ramass� ou consomm� ->
			item clignote avant de disparaitre
			
		- text info se scale en fonction de sa proportion dans la fenetre ?!
	- changer le skin du perso pour le faire pencher en arri�re
	- rajouter d'autres backgrounds?!
design:
	- problem de porte acc�s involontaire au porte en longeant les murs
	- ennemy se retourn de fa�on smooth?
	
	- gagner les items plutot que les ramasser ?! quand on tue un ennemi avec un �p�e ?!
	(cl� : battre son score de 3 salles d'un coup,
	 bouclier : passer 5 salles sans perdre de la vie,
	 cadenas : repasser les salles d�j� parcourue sans se tromper (fait aussi regagner de la vie))

architecture:
	- ennemis change de pattern en temps r�el ?
		(- chaque ennemi change de pattern quand il a fini
		- choisi le pattern en avancant croissant dans le tableau de pattern (comme �a chaque ennemi prendra toujours le m�me?!))

------
- sauver le score

*/


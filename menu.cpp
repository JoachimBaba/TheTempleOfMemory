#include "flib.h"
#include "flib_vec2.h"

#include "square.h"
#include "interface.h"
#include "screen_overlay\screen-overlay.h"

#include "menu.h"
#include "constants.h"
#include "trophy.h"
#include "sound.h"

TGfxTexture * TMenu::s_pTexture[EMenuPage_Count];

TMenu::TMenu()
	: m_eActualPage( EMenuPage_Main ), m_eNextPage( EMenuPage_Main )//, m_pScreenOverlay( nullptr )
{
	m_pScreenOverlay = new TScreenOverlay( EScreenOverlayState_AllBlack, fFADE_DURATION );

	for( int i = 0; i < EMenuPage_Count; ++i )
	{
		switch( i )
		{
		case EMenuPage_Main:
		{
			m_pPage[i] = new TInterface( MainMenuSettup );
			
			s_pTexture[i] = GfxTextureLoad( "mainMenuBg.tga" );
			m_pBackground[i] = GfxSpriteCreate(s_pTexture[i]);
		}
			break;
		case EMenuPage_Instructions:
		{
			m_pPage[i] = new TInterface( MenuPageSettup );

			s_pTexture[i] = GfxTextureLoad( "InstructionsBg.tga" );
			m_pBackground[i] = GfxSpriteCreate( s_pTexture[i] );
		}
			break;
		case EMenuPage_Trophies:
		{
			m_pPage[i] = new TInterface(MenuPageSettup);

			s_pTexture[i] = GfxTextureLoad("BasicPageBg.tga");
			m_pBackground[i] = GfxSpriteCreate(s_pTexture[i]);
		}
			break;
		default:
			m_pPage[i] = nullptr;
			s_pTexture[i] = nullptr;
			m_pBackground[i] = nullptr;
			break;
		}		
	}
}

TMenu::~TMenu()
{
	for( int i = 0; i < EMenuPage_Count; i++ )
	{
		delete m_pPage[i];
		if( s_pTexture[i] != nullptr )
			GfxTextureDestroy( s_pTexture[i] );
		if(m_pBackground[i] != nullptr )
			GfxSpriteDestroy( m_pBackground[i] );
	}
	delete m_pScreenOverlay;
}

bool TMenu::Update()
{
	//button test
	if ( m_pScreenOverlay->GetState() == EScreenOverlayState_AllTranslucent )
	{
		//Check Menu Buttons::::::::::::::::::::::::::::::::::::::::::::::::::::::
		int iResult = m_pPage[m_eActualPage]->CheckInput();
		if(iResult != BUTTON_NULL)
		{
			//fade image if a button is selected (problematic if a button has no use -> we go out of the menu )
			if( iResult != BUTTON_NULL )
			{
				m_pScreenOverlay->Start(EScreenOverlayState_FadingOut);

				TSoundSystem::Play( ESound_MenuClick );
			}

			//buttons actions
			switch( m_eActualPage )
			{
				//main page
			case EMenuPage_Main:
			{
				switch( iResult )
				{
				case EMainMenuState_Play:
					m_eNextPage = EMenuPage_Null;
					GfxDbgPrintf("main->game %d\n", iResult);
					break;
				case EMainMenuState_Instructions:
					m_eNextPage = EMenuPage_Instructions;
					GfxDbgPrintf( "main->instruct %d\n", iResult );
					break;
				case EMainMenuState_Trophies:
					m_eNextPage = EMenuPage_Trophies;
					GfxDbgPrintf("main->trophies %d\n", iResult);
					break;
				default:
					break;
				}
			}
				break;

				//instruction page
			case EMenuPage_Instructions:
				if (iResult == 0)// back to menu button (the only button)
				{
					m_eNextPage = EMenuPage_Main;
					GfxDbgPrintf("instruc->main %d\n", iResult);
				}
				break;

				//trophy page
			case EMenuPage_Trophies:
				if (iResult == 0)// back to menu button (the only button)
				{
					m_eNextPage = EMenuPage_Main;
					GfxDbgPrintf("instruc->main %d\n", iResult);
				}
				break;

			default:
				break;
			}
		}
		//Check Page::::::::::::::::::::::::::::::::::::::
		if( m_eActualPage == EMenuPage_Trophies )
		{
			TTrophy::Update();
		}
	}
	else
	{
		//fade update
		EScreenOverlayState eFadeState = m_pScreenOverlay->Update();

		if( eFadeState == EScreenOverlayState_AllTranslucent )
		{
			m_eNextPage = EMenuPage_Null;
			GfxDbgPrintf( "end fade\n" );
		}
		else if( eFadeState == EScreenOverlayState_AllBlack )
		{
			if( m_eNextPage != EMenuPage_Null )
			{
				m_eActualPage = m_eNextPage;
			}
			else
			{
				m_eNextPage = m_eActualPage;//next time we're back to the menu it will be on the page that we left
				return false;//happens if we go out of the menu to another mode (play in this case)
			}

			m_pScreenOverlay->Start( EScreenOverlayState_FadingIn );
			GfxDbgPrintf( "start fade\n" );
		}
	}

	return true;
}

void TMenu::Render()
{
	if( m_eActualPage != EMenuPage_Null )
	{
		GfxSpriteRender( m_pBackground[m_eActualPage] );
		m_pPage[m_eActualPage]->Render();

		if (m_eActualPage == EMenuPage_Trophies)
			TTrophy::Render();
	}
	
	if( m_pScreenOverlay->GetState() != EScreenOverlayState_AllTranslucent )
		m_pScreenOverlay->Render();
}


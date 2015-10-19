#include <cstring>
#include <cstdlib>

#include "flib.h"
#include "flib_vec2.h"

#include "parser\xml_attribute.h"
#include "parser\xml_element.h"
#include "parser\xml_root.h"

#include "setting.h"

TSetting::TSetting()
	:m_pXmlRoot(nullptr), m_iPatternCount(0)
{
	//XML
	InitPatterns();//set all destinations to 0.f
	m_pXmlRoot = ParseXML( "rooms_info.tmx" );
	CreateRoomsModel(m_pXmlRoot);

	delete m_pXmlRoot;
	m_pXmlRoot = nullptr;
}

TSetting::~TSetting()
{
	delete m_pXmlRoot;
}


void TSetting::CreateRoomsModel(TXML_Element * pRoot)
{
	TXML_Element * pElement = nullptr;
	const char * pBranchName = "objectgroup";

	//Get first branch
	pElement = pRoot->FindFirstChildElement(pBranchName);
	
	while (pElement != nullptr && m_iPatternCount < PATTERN_MAX)
	{
		//search branch's content
		TXML_Attribute * pNameAttribute = pElement->FindAttribute("name");

		while (pNameAttribute != nullptr)
		{
			const char * pElementName = pNameAttribute->GetValueString();

			if (strcmp(pElementName, "pattern") == 0)//if objectgroup is a pattern
			{
				LoadIds( pElement );//save path's id
				LoadPattern( pElement );//save Ennemies's path
				m_iPatternCount++;
				pNameAttribute = nullptr;
			}
			else
			{
				pNameAttribute = pNameAttribute->GetNextItem();
			}
		}

		//go to next branch
		pElement = pElement->FindNextSiblingElement(pBranchName);
	}
}

void TSetting::LoadPattern( TXML_Element * pElement )
{
	int iEnnemy = 0;
	TXML_Element * pObject = nullptr;
	pObject = pElement->FindFirstChildElement("object");

	while (pObject != nullptr && iEnnemy < ENNEMY_MAX)
	{
		//get global position
		TGfxVec2 tObjectPos;
		tObjectPos.x = pObject->FindAttribute("x")->GetValueNumeric();
		tObjectPos.y = pObject->FindAttribute("y")->GetValueNumeric();

		//get path destinations
		TXML_Element * pPolyline = nullptr;
		pPolyline = pObject->FindFirstChildElement("polyline");

		if (pPolyline != nullptr)
		{
			TXML_Attribute * pPoints;
			pPoints = pPolyline->FindAttribute("points");

			if (pPoints != nullptr)
			{
				const char * pPointsValue = pPoints->GetValueString();
				int iDestCpt = 0;

				char pDestinations[MAX_SIZE_ATTRIBUTE_VALUE];
				strcpy(pDestinations, pPointsValue);

				while( pDestinations[0] != '\0' && iDestCpt < STEP_MAX)
				{
					m_pEnnemiesPatterns[m_iPatternCount][iEnnemy][iDestCpt] = TGfxVec2( 0.f, 0.f );
					TGfxVec2 &pDest = m_pEnnemiesPatterns[m_iPatternCount][iEnnemy][iDestCpt];

					//X coord---------------------------
					const char * pDestXEnd = strchr(pDestinations, ',');

					//first coord extraction
					int iSizeX = pDestXEnd - (pDestinations);//Dest Size
					char pDestX[MAX_SIZE_ATTRIBUTE_VALUE];
					strncpy(pDestX, pDestinations, iSizeX);
					pDestX[iSizeX] = '\0';

					//coord convert
					pDest.x = ( float ) atof( pDestX );
					pDest.x += tObjectPos.x;

					//string update
					strcpy(pDestinations, pDestXEnd+1);//+1 to skip ','

					//Y coord------------------------------
					const char * pDestYEnd = strchr(pDestinations, ' ');
					if (pDestYEnd == nullptr)
					{
						//tDests[iDestCpt].y = 0;
						//GfxDbgPrintf("ERROR Y coord missing\n");
						pDestYEnd = strchr(pDestinations, '\0');
					}

					//second coord extraction
					int iSizeY = pDestYEnd - (pDestinations);//Dest Size
					char pDestY[MAX_SIZE_ATTRIBUTE_VALUE];
					strncpy(pDestY, pDestinations, iSizeY);
					pDestY[iSizeY] = '\0';

					//coord convert
					pDest.y = ( float ) atof( pDestY );
					pDest.y += tObjectPos.y;

					//string update
					strcpy(pDestinations, pDestYEnd);

					iDestCpt++;
				}
			}
		}
		//go to next object
		pObject = pObject->FindNextSiblingElement("object");
		iEnnemy++;
	}
}

void TSetting::LoadIds( TXML_Element * pElement )
{
	TXML_Element * pProperty = pElement->FindFirstChildElement( "properties" )->FindFirstChildElement( "property" );

	if( pProperty != nullptr )
	{
		m_iPatternsId[m_iPatternCount] = (int)pProperty->FindAttribute( "value" )->GetValueNumeric();
	}
}

void TSetting::InitPatterns()
{
	for( int a = 0; a < PATTERN_MAX; a++ )
	{
		for( int b = 0; b < ENNEMY_MAX; b++ )
		{
			for( int c = 0; c < STEP_MAX; c++ )
			{
				m_pEnnemiesPatterns[a][b][c] = TGfxVec2(-1.f, -1.f);//ennemies destinations cant be out of the screen so its never going to be below 0.f (-1.f mean not used)
			}
		}
		m_iPatternsId[a] = -1;
	}
}

int TSetting::GetEnnemyCount(int iPattern)const
{
	int i = 0;
	for (i = 0; i <= ENNEMY_MAX; ++i)
	{
		if (m_pEnnemiesPatterns[iPattern][i][0].x < 0.f)//coordinate of a valid destination can't be below 0.f
			return i;
	}
	return i;
}

int TSetting::GetDestinationsCount(int iPattern, int iEnnemy)
{
	int i = 0;
	for (int i = 0; i <= STEP_MAX; ++i)
	{
		if (m_pEnnemiesPatterns[iPattern][iEnnemy][i].x < 0.f)
			return i;
	}
	return i;
}

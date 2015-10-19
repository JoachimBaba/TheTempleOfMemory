#include <cstring>
#include <ctype.h>

#include "sll_item.h"
#include "sll_root.h"
#include "xml_element.h"
#include "xml_attribute.h"
#include "xml_root.h"

#include "flib.h"

TXML_Root::TXML_Root()
	: TXML_Element()
{
	//ParseXML( pName );
	//DebugPrint(this);
}

TXML_Root::~TXML_Root()
{

}

TXML_Element * TXML_Root::FindFirstElement(const char * pName)
{
	TXML_Element * pActualElement = GetChildList()->GetFirstItem();

	if (pActualElement != nullptr)
	{
		if (strcmp(pActualElement->GetName(), pName) == 0)
		{
			pActualElement = pActualElement->FindNextSiblingElement(pName);
		}
	}
	return pActualElement;
}

TXML_Root * ParseXML( const char * pFileName )
{
	//get file
	TGfxFile * pFile;
	pFile = GfxFileOpenRead(pFileName);

	//create buffer
	int iFileSize = GfxFileSize(pFile);
	char * pBuffer = static_cast<char *>(GfxMemAlloc(iFileSize + 1));

	//init buffer content
	GfxFileRead(pFile, pBuffer, iFileSize);
	pBuffer[iFileSize] = '\0';
	/*
	//print content
	int i = 0;
	while (pBuffer[i] != '\0')
	{
		GfxDbgPrintf("%c", pBuffer[i]);
		++i;
	}
	*/
	GfxDbgPrintf("\n\n");

	//parse
	int iMainCursor = 0;
	int iLevel = 0;

	TXML_Root * pRoot = new TXML_Root();

	while (pBuffer != nullptr)
	{
		//parse first branch
		iMainCursor = Parse( pBuffer, iMainCursor, pRoot, iLevel );
	}
	
	//free memeory
	GfxMemFree(pBuffer);
	GfxFileClose(pFile);

	return pRoot;
}

int Parse( char *& pBuffer, int iCursor, TXML_Element * pBranchElement, int & iHierarchyLevel )
{
	char sElementName[MAX_SIZE_ELEMENT_NAME];
	ETagType eTagType = ETagType_Opening;

	if (pBuffer[iCursor] != '<')
	{
		pBuffer = strchr(pBuffer, '<');
		iCursor = 0;
		if( pBuffer == nullptr )
			return iCursor;//assert?!
	}

	while (pBuffer[iCursor] != '\0')
	{
		//skip space
		iCursor = SkipSpace(pBuffer, iCursor);

		if (pBuffer[iCursor] == '<')//if we look for an opening tag
		{

			if (pBuffer[iCursor + 1] == '?' ||//info tag
				pBuffer[iCursor + 1] == '!')//comment tag
			{
				++iCursor;
				eTagType = ETagType_InfoOrComment;
				//skip this tag

				pBuffer = strchr(pBuffer + iCursor, '<');
				iCursor = 0;

				//delete pBranchElement;//element invalid
				//pBranchElement = nullptr;
			}
			else if (pBuffer[iCursor + 1] == '/')//closing tag
			{
				iCursor++;//skip '<'
				eTagType = ETagType_Closing;

				iCursor++;//skip '/'

				char sClosingName[MAX_SIZE_ELEMENT_NAME];
				iCursor = Scan(pBuffer, iCursor, EScanType_TagName, sClosingName, nullptr);

				if (strcmp(sElementName, sClosingName) == 0)
				{
					GfxDbgPrintf("closing tag match %s == %s\n", sElementName, sClosingName);
					iHierarchyLevel--;

					//check if there is content
					if( pBuffer[0] != '\0' )
					{
						char * pCheckBuffer = strchr( pBuffer + iCursor, '<' );

						pBuffer = pCheckBuffer;
						pCheckBuffer = nullptr;
					}
				}
				else
				{
					GfxDbgPrintf( "ERROR : closing tag doesn't match %s != %s\n", sElementName, sClosingName );
					//GfxDbgAssert(false, "ending tag name doesn't match.");
					delete pBranchElement;
					pBranchElement = nullptr;
				}
				iCursor = 0;
				return iCursor;
			}
			else if (pBuffer[iCursor + 1] != '/')//opening tag
			{
				iCursor++;

				//check if its autoclosing
				char * pCheckBuffer = strchr(pBuffer + iCursor, '>');

				if (pCheckBuffer[-1] == '/')
					eTagType = ETagType_AutoClosing;
				else
					eTagType = ETagType_Opening;

				pCheckBuffer = nullptr;

				//create Element if needed
				if( pBranchElement == nullptr)
					pBranchElement = new TXML_Element();

				//scan
				iCursor = Scan(pBuffer, iCursor, EScanType_TagName, sElementName, pBranchElement);
				
				if( eTagType != ETagType_AutoClosing )
				{
					//check if there is content
					pCheckBuffer = strchr(pBuffer + iCursor, '<');

					while (pCheckBuffer[1] != '/')//if next is not closing tag (there is content)
					{
						TXML_Element * pChildElement = new TXML_Element();

						Parse(pBuffer, iCursor, pChildElement, ++iHierarchyLevel);//tmp -> value return by Parse not used : could it be set to iCursor

						//store child element
						if( pChildElement != nullptr )
							pBranchElement->GetChildList()->Append( pChildElement );
						else
							delete pChildElement;

						pCheckBuffer = pBuffer;
					}
					//possible value to tag ? --> handled in method scan
					pBuffer = pCheckBuffer;
					pCheckBuffer = nullptr;
				}
				else
				{
					iHierarchyLevel--;
					return iCursor;
				}
			}

			GfxDbgPrintf("TagType : %d\n\n", eTagType);
		}
		else
		{
			GfxDbgPrintf( "char other than '<' before an opening tag ");
		}
		iCursor = 0;
	}

	if( pBuffer[iCursor] == '\0' && iHierarchyLevel+1 > 0 && eTagType == ETagType_Opening )
		GfxDbgPrintf( "ERROR : Reached EOF while parsing tag\n" );

	return iCursor;
}

int Scan(char *& pBuffer, int iCursor, EScanType eType, char * sElementName, TXML_Element * pBranchElement)
{
	char pTagName[MAX_SIZE_ELEMENT_NAME];
	int iInfoCount = 0;
	bool bEndName = false;

	SkipSpace(pBuffer, iCursor);//skip space

	//scan tag name
	while (pBuffer[iCursor] != '\0')//check for tag ending
	{
		switch (eType)
		{
		case EScanType_TagName:
		{
			if (pBuffer[iCursor] == '=')//correct if there is no space 
				GfxDbgPrintf( "missing space between tag name & attribute name" );

			if (isspace(pBuffer[iCursor]) != 0 ||
				pBuffer[iCursor] == '>')
			{
				pTagName[iInfoCount] = '\0';
				GfxDbgPrintf("tag name : %s\n", pTagName);

				//store element name
				strcpy(sElementName, pTagName);

				if (pBranchElement != nullptr)//If we want to scan a closing tag we dont want to overwrite the element name, so pBranchElement is nullptr
					pBranchElement->SetName(sElementName);

				//skip spaces after
				iCursor = SkipSpace(pBuffer, iCursor);//skip space

				if (pBuffer[iCursor] != '>')
				{
					iCursor = Scan(pBuffer, iCursor, EScanType_AttributeName, sElementName, pBranchElement);
				}
				else
				{
					//check if its a closing tag or if there is a tag value after this tag

					iCursor = SkipSpace(pBuffer, ++iCursor);

					//if( pBuffer[iCursor] == '<' && pBuffer[iCursor + 1] == '/' )
					if( pBuffer[iCursor] != '<' )
						iCursor = Scan(pBuffer, iCursor, EScanType_TagValue, sElementName, pBranchElement);					
				}
				bEndName = true;
			}
		}
			break;
		case EScanType_TagValue:
		{
			if( isspace( pBuffer[iCursor] ) != 0 )//systematic concatenation
				iCursor = SkipSpace( pBuffer, iCursor );

			if (pBuffer[iCursor] == '<' )
			{
				pTagName[iInfoCount] = '\0';
				GfxDbgPrintf("tag value : %s\n", pTagName);

				//store element name
				pBranchElement->SetValue(pTagName);

				bEndName = true;
			}
		}
			break;
		case EScanType_AttributeName:
		{
			if (pBuffer[iCursor] == '"')//correct if there is no space 
				GfxDbgPrintf( "missing '=' between attrib name & value" );

			if (isspace(pBuffer[iCursor]) != 0 ||
				pBuffer[iCursor] == '=' ||
				pBuffer[iCursor] == '>')
			{
				pTagName[iInfoCount] = '\0';
				GfxDbgPrintf("attrib name : %s\n", pTagName);

				TXML_Attribute * pNewAttribute = new TXML_Attribute();
				pNewAttribute->SetName(pTagName);

				pBranchElement->GetAttributesList()->Append(pNewAttribute);

				//skip spaces after
				iCursor++;
				iCursor = SkipSpace(pBuffer, iCursor);//skip space

				pBuffer = strchr(pBuffer + iCursor, '"');
				iCursor = 1;

				iCursor = Scan(pBuffer, iCursor, EScanType_AttributeValue, sElementName, pBranchElement);

				bEndName = true;
			}
		}
			break;
		case EScanType_AttributeValue:
		{
			if (pBuffer[iCursor] == '"')
			{
				pTagName[iInfoCount] = '\0';
				GfxDbgPrintf("value : %s\n", pTagName);

				pBranchElement->GetAttributesList()->FindLastItem()->SetValue(pTagName);

				iCursor++;

				//skip spaces after
				iCursor = SkipSpace(pBuffer, iCursor);//skip space

				
				if( pBuffer[iCursor] != '>' && pBuffer[iCursor] != '/' )
				{
					iCursor = Scan(pBuffer, iCursor, EScanType_AttributeName, sElementName, pBranchElement);
				}

				if( pBuffer[iCursor] == '/' )//get out of autoclosing tag
				{
					pBuffer = strchr(pBuffer + iCursor, '>');
					iCursor = 0;
					iCursor = SkipSpace(pBuffer, ++iCursor);
					pBuffer = pBuffer + iCursor;
				}

				bEndName = true;
			}
		}
			break;
		}

		if (!bEndName)
		{
			//store char
			pTagName[iInfoCount++] = pBuffer[iCursor++];

			GfxDbgAssert( iInfoCount < MAX_SIZE_ELEMENT_NAME, "tag name too long" );
		}
		else
		{
			return iCursor;
		}
	}
	if( pBuffer[iCursor] == '\0' )
		GfxDbgPrintf( "Reached EOF while scanning tag\n" );

	return iCursor;
}

int SkipSpace(char *& pBuffer, int iCursor)
{
	while( isspace( pBuffer[iCursor] ))
	{
		iCursor++;
	}

	return iCursor;
}

void DebugPrint(TXML_Element * pElement)
{
	//Element
	GfxDbgPrintf("Element Name : %s\n", pElement->GetName());

	if (pElement->IsValueNumeric())
		GfxDbgPrintf("Element Numeric Value : %f\n", pElement->GetValueNumeric());
	else
		GfxDbgPrintf("Element Value : %s\n\n", pElement->GetValueString());

	//Element's attributes
	TXML_Attribute * pActualAttrib = pElement->GetAttributesList()->GetFirstItem();
	while (pActualAttrib != nullptr)
	{
		GfxDbgPrintf("Attribute Name : %s\n", pActualAttrib->GetName());

		if (pActualAttrib->IsValueNumeric())
			GfxDbgPrintf("Attribute Numeric Value : %f\n", pActualAttrib->GetValueNumeric());
		else
			GfxDbgPrintf("Attribute Value : %s\n", pActualAttrib->GetValueString());

		pActualAttrib = pActualAttrib->GetNextItem();
	}
	GfxDbgPrintf("--------------------------------\n\n");

	//Element's childs
	if (pElement->GetChildList() != nullptr)
	{
		GfxDbgPrintf("Childs Elements\n");
		pElement->GetChildList()->Iterate(DebugPrint);
	}

	//Element's Sibbiling
	TXML_Element * pNextItem = pElement->GetNextItem();
	while (pNextItem != nullptr)
	{
		GfxDbgPrintf("Next Element\n");
		DebugPrint(pNextItem);
		pNextItem = pNextItem->GetNextItem();
	}
}


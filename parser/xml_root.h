#pragma once

#include "xml_element.h"

template <class T>
class TSLL_Root;

enum EScanType
{
	EScanType_TagName,
	EScanType_TagValue,
	EScanType_AttributeName,
	EScanType_AttributeValue
};

enum ETagType
{
	ETagType_Opening,
	ETagType_Closing,
	ETagType_AutoClosing,
	ETagType_InfoOrComment
};


class TXML_Root : public TXML_Element
{
public:
	TXML_Root();
	virtual ~TXML_Root();

	TXML_Element * FindFirstElement(const char * pName);
};

void DebugPrint( TXML_Element * pElement );
TXML_Root * ParseXML( const char * pFileName );
int SkipSpace( char *& pBuffer, int iCursor );
int Parse( char *& pBuffer, int iCursor, TXML_Element * pBranchElement, int & iHierarchyLevel );
int Scan( char *& pBuffer, int iCursor, EScanType eType, char * sElementName, TXML_Element * pBranchElement );


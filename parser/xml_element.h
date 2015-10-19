#pragma once

#include "sll_item.h"

template <class T>
class TSLL_Root;

class TXML_Attribute;

const int MAX_SIZE_ELEMENT_NAME = 63;
const int MAX_SIZE_ELEMENT_VALUE = 63;

class TXML_Element : public TSLL_Item<TXML_Element>
{
public:
	TXML_Element();
	virtual ~TXML_Element();

	TSLL_Root<TXML_Attribute> * GetAttributesList() { return m_pAttributesList; };
	TSLL_Root<TXML_Element> * GetChildList() { return m_pElementChildsList; };

	void SetName(const char * pNewName);
	void SetValue(const char * pNewValue);

	TXML_Element * FindNextSiblingElement(const char * pName);
	TXML_Element * FindFirstChildElement(const char * pName)const;
	TXML_Attribute * FindAttribute(const  char * pName)const;

	const char * GetName()const { return m_pName; };
	const char * GetValueString()const { return m_pValue; };
	bool IsValueNumeric()const;
	float GetValueNumeric()const;
	
private:
	TSLL_Root<TXML_Element> * m_pElementChildsList;
	TSLL_Root<TXML_Attribute> * m_pAttributesList;

	char m_pName[MAX_SIZE_ELEMENT_NAME];
	char m_pValue[MAX_SIZE_ELEMENT_VALUE];
};

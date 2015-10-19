#include "flib.h"
#include <cstring>
//#include <ctype.h>
#include <cstdlib>

#include "sll_item.h"
#include "sll_root.h"

#include "xml_attribute.h"
#include "xml_element.h"


TXML_Element::TXML_Element()
: TSLL_Item(), m_pElementChildsList(TSLL_Root<TXML_Element>::Create()), m_pAttributesList(TSLL_Root<TXML_Attribute>::Create())
{
	m_pName[0] = '\0';
	m_pValue[0] = '\0';
}

TXML_Element::~TXML_Element()
{
	if (m_pElementChildsList != nullptr)
		delete m_pElementChildsList;
	if (m_pAttributesList != nullptr)
		delete m_pAttributesList;
}

TXML_Element * TXML_Element::FindNextSiblingElement(const char * pSearchedName)
{
	TXML_Element * pActualElement = nullptr;
	pActualElement = GetNextItem();

	while (pActualElement != nullptr)
	{
		char pActualElementName[MAX_SIZE_ELEMENT_NAME];
		strcpy(pActualElementName, pActualElement->GetName());

		if (strcmp(pActualElementName, pSearchedName) == 0)//check if searched Element's name match with current item
		{
			return pActualElement;
		}
		else
		{
			pActualElement = pActualElement->GetNextItem();
		}
	}
	return pActualElement;
}

TXML_Element * TXML_Element::FindFirstChildElement(const char * pName)const
{
	char pActualChildName[MAX_SIZE_ELEMENT_NAME];
	TXML_Element * pActualElement = m_pElementChildsList->GetFirstItem();
	
	while (pActualElement != nullptr)
	{
		strcpy(pActualChildName, pActualElement->GetName());

		if (strcmp(pActualChildName, pName) == 0)//check if searched ChildElement's name match with current item
		{
			return pActualElement;
		}
		else
		{
			pActualElement = pActualElement->GetNextItem();
		}
	}
	return pActualElement;
}

TXML_Attribute * TXML_Element::FindAttribute(const char * pName)const
{
	char pActualAttributeName[MAX_SIZE_ATTRIBUTE_NAME];
	TXML_Attribute * pActualAttribute = m_pAttributesList->GetFirstItem();

	while (pActualAttribute != nullptr)
	{
		strcpy(pActualAttributeName, pActualAttribute->GetName());

		if (strcmp(pActualAttributeName, pName) == 0)//check if searched Attribute's name match with current item
		{
			return pActualAttribute;
		}
		else
		{
			pActualAttribute = pActualAttribute->GetNextItem();
		}
	}
	return pActualAttribute;
}

bool TXML_Element::IsValueNumeric()const
{
	float fConvert = (float)atof(m_pValue);

	if (fConvert > 0.0f || fConvert < 0.0f)
		return true;

	return false;
}

float TXML_Element::GetValueNumeric()const
{
	return (float)atof(m_pValue);
};

void TXML_Element::SetName(const char * pNewName)
{
	strncpy(m_pName, pNewName, MAX_SIZE_ELEMENT_VALUE);
}

void TXML_Element::SetValue(const char * pNewValue)
{
	strncpy(m_pValue, pNewValue, MAX_SIZE_ELEMENT_VALUE);
}




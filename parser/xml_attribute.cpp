#include "flib.h"
#include <cstring>
//#include <ctype.h>
#include <cstdlib>

#include "sll_item.h"
#include "sll_root.h"

#include "xml_element.h"
#include "xml_attribute.h"

TXML_Attribute::TXML_Attribute()
	: TSLL_Item()
{
	m_pName[0] = '\0';
	m_pValue[0] = '\0';
}

TXML_Attribute::~TXML_Attribute()
{

}

bool TXML_Attribute::IsValueNumeric()const
{
	float fConvert = (float)atof(m_pValue);

	if (fConvert > 0.0f || fConvert < 0.0f)
		return true;

	return false;
}

float TXML_Attribute::GetValueNumeric()const
{
	return (float)atof(m_pValue);
};

void TXML_Attribute::SetName(const char * pNewName)
{
	strncpy(m_pName, pNewName, MAX_SIZE_ATTRIBUTE_VALUE);
}

void TXML_Attribute::SetValue(const char * pNewValue)
{
	strncpy(m_pValue, pNewValue, MAX_SIZE_ATTRIBUTE_VALUE);
}

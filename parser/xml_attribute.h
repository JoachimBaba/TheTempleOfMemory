#pragma once

#include "sll_item.h"

const int MAX_SIZE_ATTRIBUTE_NAME = 63;
const int MAX_SIZE_ATTRIBUTE_VALUE = 63;

class TXML_Attribute : public TSLL_Item<TXML_Attribute>
{
public:
	TXML_Attribute();
	virtual ~TXML_Attribute();

	void SetName(const char * pNewName);
	void SetValue(const char * pNewValue);

	const char * GetName()const { return m_pName; };
	const char * GetValueString()const { return m_pValue; };
	bool IsValueNumeric()const;
	float GetValueNumeric()const;

private:
	char m_pName[MAX_SIZE_ATTRIBUTE_NAME];
	char m_pValue[MAX_SIZE_ATTRIBUTE_VALUE];
};



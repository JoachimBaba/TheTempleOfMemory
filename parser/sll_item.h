#pragma once

template <class T>
class TSLL_Item
{
public:
	TSLL_Item();
	virtual ~TSLL_Item();

	//Method
	void InsertAfter(T * pNewItem);
	
	//Get/Set
	void SetNextItem(T * pNewNextItem) { m_pNextItem = pNewNextItem; };
	T * GetNextItem()const{ return m_pNextItem; };

private:
	T * m_pNextItem;
};

template <class T>
TSLL_Item<T>::TSLL_Item()
: m_pNextItem(nullptr)
{

}

template <class T>
TSLL_Item<T>::~TSLL_Item()
{
	if (m_pNextItem != nullptr)
		delete m_pNextItem;
}

template <class T>
void TSLL_Item<T>::InsertAfter(T * pNewItem)
{
	if (m_pNextItem != nullptr)
	{
		//link actual next to new next
		pNewItem->SetNextItem(m_pNextItem);
	}
	//link this item to new next
	m_pNextItem = pNewItem;
}


#pragma once

template <class T>
class TSLL_Item;

template <class T>
class TSLL_Root
{
public:
	static TSLL_Root * Create(){ return new TSLL_Root(); };
	TSLL_Root();
	TSLL_Root(T * pFirstItem);
	virtual ~TSLL_Root();

	T * GetFirstItem()const { return m_pFirstItem; };
	T * FindLastItem()const;

	void Append(T * pNewItem);
	bool Remove(T * pToRemoveItem);
	int GetCount()const;

	void Iterate(void (*pFunction)(T * pItem));

private:
	T * m_pFirstItem;
};


template <class T>
TSLL_Root<T>::TSLL_Root()
: m_pFirstItem(nullptr)
{

}

template <class T>
TSLL_Root<T>::TSLL_Root(T * pFirstItem)
: m_pFirstItem(pFirstItem)
{

}

template <class T>
TSLL_Root<T>::~TSLL_Root()
{
	delete m_pFirstItem;
}

template <class T>
void TSLL_Root<T>::Append(T * pNewItem)
{
	T * pLastItem = FindLastItem();

	//if there is a last item
	if (pLastItem != nullptr)
		pLastItem->SetNextItem(pNewItem);//the new is linked to the last
	else
		m_pFirstItem = pNewItem;//the new become the first
}

template <class T>
T * TSLL_Root<T>::FindLastItem()const
{
	T * pNextItem = m_pFirstItem;

	if (pNextItem != nullptr)
	{
		//while there is a next item
		while (pNextItem->GetNextItem() != nullptr)
		{
			pNextItem = pNextItem->GetNextItem();//get its own next item
		}
	}
	return pNextItem;
}

template <class T>
int TSLL_Root<T>::GetCount()const
{
	int i = 0;

	T * pNextItem = m_pFirstItem;

	//while there is a next item
	while (pNextItem != nullptr)
	{
		pNextItem = pNextItem->GetNextItem();//get its own next item
		i++;//count it
	}
	return i;
}

template <class T>
bool TSLL_Root<T>::Remove(T * pToRemoveItem)
{
	T * pActualItem = m_pFirstItem;
	if (pActualItem != nullptr)
	{
		//if the address of the item to remove match with the first
		if (pActualItem == pToRemoveItem)
		{
			m_pFirstItem = m_pFirstItem->GetNextItem();//second become the first

			//remove the first (still store at pActualItem's address)
			delete pActualItem;
			pActualItem = nullptr;
			return true;
		}
		else
		{
			T * pNextItem = pActualItem->GetNextItem();

			
			while (pNextItem != pToRemoveItem &&//if the address of the item to remove doesn't match with the current
				pNextItem != nullptr)//as long as the end of the list isn't reached
			{
				pNextItem = pNextItem->GetNextItem();//go on with next item
			}

			/*
			if after the search there is an item
			(which means it must be the one to remove)
			remove it
			*/
			if (pNextItem != nullptr)
			{
				delete pActualItem;
				pActualItem = nullptr;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

template <class T>
void TSLL_Root<T>::Iterate(void (*pFunction)(T * pItem))
{
	T * pActualItem = m_pFirstItem;

	//call pFunction as long as there is items
	while (pActualItem != nullptr)
	{
		pFunction(pActualItem);
		pActualItem = pActualItem->GetNextItem();
	}
}


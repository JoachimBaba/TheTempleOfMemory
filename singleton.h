#pragma once

template <class T>
class TSingleton
{
public:
	static T * CreateInstance();
	static void DestroyInstance();

	//static T * GetInstance(){ return T; };
protected:
	TSingleton();
	virtual ~TSingleton();

	static T * s_pInstance;
};

template <class T>
T * TSingleton<T>::s_pInstance = nullptr;

template <class T>
T * TSingleton<T>::CreateInstance()
{
	if( s_pInstance == nullptr )
	{
		s_pInstance = new T();
		return s_pInstance;
	}
	return nullptr;
}

template <class T>
void TSingleton<T>::DestroyInstance()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

template <class T>
TSingleton<T>::TSingleton()
{

}

template <class T>
TSingleton<T>::~TSingleton()
{

}


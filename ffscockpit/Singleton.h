#pragma once

template<class T>
class CSingleton
{

public:
	static T* Instance (){
		if(!m_pInstance) m_pInstance = new T;
		return m_pInstance;
	}
	static void Free (){
		delete m_pInstance;
		m_pInstance = NULL;
	}
protected:
	CSingleton (){}
	~CSingleton (){}
private:
	CSingleton (CSingleton const&);
	CSingleton& operator=(CSingleton const&);

	static T* m_pInstance;
};

template<class T> T* CSingleton<T>::m_pInstance = NULL;

#ifndef SINGLETON_H
#define SINGLETON_H

#include <thread>

template<class T>
class Singleton{
protected:
	Singleton(){}
	Singleton(const Singleton&);
	Singleton& operator=(Singleton&);
	~Singleton(){}
	static T* instance;
	static void createInstance(){
		instance = new T();
	}

	static std::once_flag instanceFlag;

public:
	static T& getInstance(){
		std::call_once(instanceFlag, createInstance);
		return *instance;
	}
};

template<typename T>
T* Singleton<T>::instance;

template<typename T>
std::once_flag Singleton<T>::instanceFlag;

#endif

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>

#include "pool.h"

class IThread{
public:
	virtual ~IThread() = 0;
	virtual void threadFunc() = 0;
};

class Thread{
private:
	IThread *pThreadObj;
	bool active;
	std::thread _thread;
	Pool<Thread>* pool;

public:
	Thread();
	~Thread();
	void init(IThread* _pThreadObj, Pool<Thread>* _pool);
	void innerThreadFunc();
};

class ThreadPool{
private:
	Pool<Thread> pool;

public:
	void initThread(IThread* _pThreadObj);
};

#endif

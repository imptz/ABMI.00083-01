#include "threadPool.h"

IThread::~IThread() {}

Thread::Thread()
	:	pThreadObj(nullptr), active(true){
	_thread = std::thread(&Thread::innerThreadFunc, this);
}

Thread::~Thread(){
	active = false;
}

void Thread::init(IThread* _pThreadObj, Pool<Thread>* _pool){
	pool = _pool;
	pThreadObj = _pThreadObj;
}

void Thread::innerThreadFunc(){
	while(active){
		while(pThreadObj == nullptr){
			if (!active)
				return;

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		pThreadObj->threadFunc();
		delete pThreadObj;
		pThreadObj = nullptr;
		pool->free(this);
		pool->print();
	}
}

void ThreadPool::initThread(IThread* _pThreadObj){
	pool.get()->init(_pThreadObj, &pool);
}

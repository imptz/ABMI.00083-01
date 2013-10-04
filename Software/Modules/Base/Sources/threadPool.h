#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>

using namespace std;

template<typename T, unsigned int initSize = 10>
class Pool{
	struct Item{
		bool free;
		T* elem;

		Item()
			:	free(true), elem(new T())
		{}

		~Item(){
			delete elem;
		}
	};

	vector<Item*> pool;

public:
	Pool(){
		for (unsigned int i = 0; i < initSize; ++i)
			pool.push_back(new Item);
	}

	~Pool(){
		for (unsigned int i = 0; i < pool.size(); ++i)
			delete pool[i];
	}

	T* get(){
		for (unsigned int i = 0; i < pool.size(); ++i){
			if (pool[i]->free){
				pool[i]->free = false;
				return pool[i]->elem;
			}
		}

		pool.push_back(new Item);
		pool[pool.size() - 1]->free = false;
		return pool[pool.size() - 1]->elem;
	}

	void free(T* item){
		for (unsigned int i = 0; i < pool.size(); ++i)
			if (pool[i]->elem == item)
				pool[i]->free = false;
	}
};

class PoolTest{
public:
	static bool test(){
		Pool<int, 4> pool;
		const unsigned int SIZE = 19;
		const unsigned int DELTA = 3;

		int* data[SIZE + DELTA];
		for (unsigned i = 0; i < SIZE; ++i){
			data[i] = pool.get();
			*(data[i]) = i * 2;
		}

		bool result = true;
		for (unsigned i = 0; i < SIZE; ++i){
			if (*(data[i]) != i * 2)
				result = false;
		}

		for (unsigned i = 0; i < SIZE + DELTA; ++i){
			pool.free(data[i]);
		}

		for (unsigned i = 0; i < SIZE + DELTA; ++i){
			data[i] = pool.get();
			*(data[i]) = i * 3;
		}

		for (unsigned i = 0; i < SIZE + DELTA; ++i){
			if (*(data[i]) != i * 3)
				result = false;
		}

		return result;
	}
};

#endif

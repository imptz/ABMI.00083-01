#include "pool.h"

bool PoolTest::test(){
	bool result = true;
	Pool<int, 2> pool;
	const unsigned int SIZE = 2;
	const unsigned int DELTA = 3;

	int* data[SIZE + DELTA];
	for (unsigned i = 0; i < SIZE; ++i){
		data[i] = pool.get();
		*(data[i]) = i * 2;
	}

	for (unsigned i = 0; i < SIZE; ++i){
		if (*(data[i]) != i * 2)
			result = false;
	}

	for (unsigned i = 0; i < SIZE; ++i){
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

//		pool.print();

	return result;
}

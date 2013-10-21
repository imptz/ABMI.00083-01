#ifndef POOL_H
#define POOL_H

#include <iostream>
#include <thread>
#include <mutex>

template<typename T, unsigned int initSize = 1>
class Pool{
private:
	std::mutex mutex;

	struct Item{
		Item* prev;
		Item* next;
		T* elem;

		Item(Item* _prev = nullptr)
			: prev(_prev), next(nullptr)
		{
			elem = new T;
		}

		~Item(){
			delete elem;
		}
	};

	Item* listFree;
	Item* listAlloc;

public:
	Pool()
		:	listFree(nullptr), listAlloc(nullptr)
	{
		allocListFree(initSize);
	}

	~Pool(){
		while(listFree != nullptr){
			Item* item = listFree->next;
			delete listFree;
			listFree = item;
		}

		while(listAlloc != nullptr){
			Item* item = listAlloc->next;
			delete listAlloc;
			listAlloc = item;
		}
	}

	static const unsigned int ALLOC_DELTA = 10;
	void allocListFree(unsigned int size = ALLOC_DELTA){
		listFree = new Item();

		Item* pItem = listFree;
		for (unsigned int i = 0; i < size - 1; ++i){
			pItem->next = new Item(pItem);
			pItem = pItem->next;
		}
	}

	T* get(){		
		std::lock_guard<std::mutex> lock(mutex);
		if (listFree == nullptr)
			allocListFree();

		Item* newListFree = listFree->next;

		
		if (listAlloc == nullptr){
			listAlloc = listFree;
			listAlloc->prev = nullptr;
			listAlloc->next = nullptr;
		}
		else{
			listAlloc->prev = listFree;
			listAlloc->prev->next = listAlloc;
			listAlloc = listFree;
		}

		listFree = newListFree;
		if (listFree != nullptr)
			listFree->prev = nullptr;

		
		return listAlloc->elem;
	}

	void free(T* elem){
		std::lock_guard<std::mutex> lock(mutex);
		Item* item = listAlloc;
		while(item != nullptr){
			if (item->elem == elem){
				if (item->prev != nullptr)
					item->prev = item->next;

				if (item->next != nullptr)
					item->next = item->prev;

				if (item == listAlloc)
					listAlloc = item->next;

				item->next = listFree;
				if (listFree != nullptr)
					listFree->prev = item;

				listFree = item;

				break;
			}

			item = item->next;
		}
	}

	void print(){
		std::lock_guard<std::mutex> lock(mutex);
		Item* item = listFree;
		while(item != nullptr){
			item = item->next;
		}

		std::cout << std::endl;

		item = listAlloc;
		while(item != nullptr){
			item = item->next;
		}
	}
};

class PoolTest{
public:
	static bool test();
};

#endif

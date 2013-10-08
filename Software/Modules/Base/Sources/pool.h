#ifndef POOL_H
#define POOL_H

#include <iostream>

using namespace std;

template<typename T, unsigned int initSize = 10>
class Pool{
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
		Item* item = listFree;
		cout << "listFree: " << item << endl;
		while(item != nullptr){
			cout << "   item = " << item << endl;
			cout << "   item->prev = " << item->prev << endl;
			cout << "   item->next = " << item->next << endl;
			cout << "   item->elem = " << *item->elem << endl << endl;
			item = item->next;
		}

		cout << endl;

		item = listAlloc;
		cout << "listAlloc: " << item << endl;
		while(item != nullptr){
			cout << "   item = " << item << endl;
			cout << "   item->prev = " << item->prev << endl;
			cout << "   item->next = " << item->next << endl;
			cout << "   item->elem = " << *item->elem << endl << endl;
			item = item->next;
		}
	}
};

class PoolTest{
public:
	static bool test();
};

#endif

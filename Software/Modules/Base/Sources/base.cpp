#include "base.h"

#include <iostream>

using namespace std;

Base::Base()	:
	id(instancesFlags.size())
{
//	cout << __FUNCTION__ << " " << id << endl;
	instancesFlags.push_back(false);
}

void Base::setInstanceFlag(){
//	cout << __FUNCTION__ << " " << id << endl;
	baseMutex.lock();
	instancesFlags[id] = true;
	baseMutex.unlock();
}

void Base::clearAllInstancesFlags(){
	for(unsigned int i = 0; i < instancesFlags.size(); ++i)
		instancesFlags[i] = false;
}

vector<bool> Base::instancesFlags;

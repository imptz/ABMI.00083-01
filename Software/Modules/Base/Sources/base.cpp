#include "base.h"

#include <iostream>

using namespace std;

Base::Base()	:
	id(instancesFlags.size())
{
	instancesFlags.push_back(false);
}

void Base::setInstanceFlag(){
	std::lock_guard<std::mutex> lock(baseMutex);
	instancesFlags[id] = true;
	baseMutex.unlock();
}

void Base::clearAllInstancesFlags(){
	for(unsigned int i = 0; i < instancesFlags.size(); ++i)
		instancesFlags[i] = false;
}

vector<bool> Base::instancesFlags;

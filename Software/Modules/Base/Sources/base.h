#ifndef BASE_H
#define BASE_H

#include <vector>
#include <mutex>

using namespace std;

class Base{
protected:
	mutex baseMutex;
public:
	static vector<bool> instancesFlags;
	unsigned int id;

	Base()	:
		id(instancesFlags.size())
	{
		cout << __FUNCTION__ << " " << id << endl;
		instancesFlags.push_back(false);
	}

	void setInstanceFlag(){
		cout << __FUNCTION__ << " " << id << endl;
		baseMutex.lock();
		instancesFlags[id] = true;
		baseMutex.unlock();
	}

	static void clearAllInstancesFlags(){
		for(unsigned int i = 0; i < instancesFlags.size(); ++i)
			instancesFlags[i] = false;
	}
};

vector<bool> Base::instancesFlags;

#endif

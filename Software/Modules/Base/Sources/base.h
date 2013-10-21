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

	Base();
	void setInstanceFlag();
	static void clearAllInstancesFlags();
};

#endif

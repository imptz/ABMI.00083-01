#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include "xml.h"

using namespace std;

class Config{
private:
	static ifstream configFile;
	static char* configData;
	static char* configDataEnd;
	static vector<XmlElement*>* elements;

public:
	static void init(const char* fileName);
	static void clear();
	static XmlElement* getElement(unsigned int index);
	static int getElementIndex(const string& _name);

private:
	static unsigned int getConfigFileSize();
	static void readConfigFile(const char* fileName);
};

#endif


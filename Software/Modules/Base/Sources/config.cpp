#include "config.h"

using namespace std;

void Config::init(const char* fileName){
	clear();
	readConfigFile(fileName);
	XmlParser::parseXml(configData, configDataEnd);
	elements = XmlParser::getElements();
}

void Config::clear(){
	if (configData != nullptr){
		delete[] configData;
	}
}

XmlElement* Config::getElement(unsigned int index){
	if (index >= elements->size())
		return nullptr;

	return (*elements)[index];
}

int Config::getElementIndex(const string& _name){
	int index = 0;
	for (;index < elements->size(); ++index)
		if ((*elements)[index]->getName() == _name)
			return index;

	return -1;
}

unsigned int Config::getConfigFileSize(){
	configFile.seekg(0, ios::beg);
	long begin = configFile.tellg();
	configFile.seekg(0, ios::end);
	return (configFile.tellg() - begin);
}

void Config::readConfigFile(const char* fileName){
	configFile.open(fileName, ifstream::in);
	if (configFile.is_open()){
		if (configData != nullptr)
			delete[] configData;

		const unsigned int CONFIG_DATA_SIZE = getConfigFileSize();
		configData = new char[CONFIG_DATA_SIZE];
		configDataEnd = configData + CONFIG_DATA_SIZE;

		configFile.seekg(0, ios::beg);

		configFile.read(configData, CONFIG_DATA_SIZE);

		configFile.close();
	}
}

ifstream Config::configFile;
char* Config::configData = nullptr;
char* Config::configDataEnd = nullptr;
vector<XmlElement*>* Config::elements;

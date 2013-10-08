#ifndef XML_H
#define XML_H

#include <map>
#include <vector>
#include <string>
#include <stack>
#include "xmlExceptions.h"

using namespace std;

class XmlElement{
private:
	vector<XmlElement*> elements;
	string name;
	map<string, string> attributes;

public:
	XmlElement();
	~XmlElement();
	void clear();
	void addElement(XmlElement* element);
	void setName(const string& _name);
	void addAttribute(const string& _name, const string& value);
	unsigned int getElementCount();
	XmlElement* getElement(unsigned int index);
	int getElementIndex(const string& _name);
	string& getName();
	bool isAttributeExists(const string& _name);
	const string& getAttributeValue(const string& _name);
	void print(string start = "");
};

class XmlParser{
private:
	static void (*stateFunc)();

	static char* source;
	static char* sourceEnd;
	static unsigned int columnNumber;
	static unsigned int stringNumber;
	static unsigned int prevColumnNumber;
	static unsigned int prevStringNumber;
	static XmlElement* element;

	static vector<XmlElement*> elements;

public:
	static vector<XmlElement*>* getElements();
	static stack<XmlElement*> elementsStack;

	static void clearElements();
	static void clearElementsStack();
	static bool isElementOnTopStack(XmlElement* _element);
	static inline bool isSourceComplete();
	static inline bool testSymbol(const char& symbol);
	static inline bool testSymbolAndNextPosition(const char& symbol);
	static inline void nextPosition();
	static inline void prevPosition();
	static inline bool isBadNameAttributeSymbol(const char& symbol);
	static string parseName();
	static pair<string, string> parseAttribute();
	static void parseCommentAndSpace();
	static void state1();
	static void state2();
	static void state3();
	static void state4();
	static void state5();
	static void state6();
	static void state7();
	static void print();

public:
	static void parseXml(char* sourceStart, char* _sourceEnd);
};

#endif

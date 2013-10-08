#include "xml.h"

#include <iostream>

using namespace std;

XmlElement::XmlElement()
	:	name("")
{}

XmlElement::~XmlElement(){
	clear();
}

void XmlElement::clear(){
	for(vector<XmlElement*>::iterator it = elements.begin() ; it != elements.end(); ++it)
		delete *it;

	elements.clear();
	attributes.clear();
}

void XmlElement::addElement(XmlElement* element){
	try{
		elements.push_back(element);
	}
	catch (std::bad_alloc& e){
		delete element;
	}
}

void XmlElement::setName(const string& _name){
	name = _name;
}

void XmlElement::addAttribute(const string& _name, const string& value){
	attributes[_name] = value;
}

unsigned int XmlElement::getElementCount(){
	return elements.size();
}

XmlElement* XmlElement::getElement(unsigned int index){
	if (index >= elements.size())
		return nullptr;

	return elements[index];
}

int XmlElement::getElementIndex(const string& _name){
	int index = 0;
	for (;index < getElementCount(); ++index)
		if (elements[index]->getName() == name)
			return index;

	return -1;
}

string& XmlElement::getName(){
	return name;
}

bool XmlElement::isAttributeExists(const string& _name){
	return (attributes.find(_name) != attributes.end());
}

const string& XmlElement::getAttributeValue(const string& _name){
	return attributes[_name];
}

void XmlElement::print(string start){
	cout << start << name << endl;
	for (map<string, string>::iterator it = attributes.begin(); it != attributes.end(); ++it)
		cout << start << "	" << it->first << " = " << it->second << endl;

	for (unsigned int i = 0; i < elements.size(); ++i)
		elements[i]->print(start + "	");
}

vector<XmlElement*>* XmlParser::getElements(){
	return &elements;
}

void XmlParser::clearElements(){
	for(vector<XmlElement*>::iterator it = elements.begin() ; it != elements.end(); ++it)
		delete *it;
}

void XmlParser::clearElementsStack(){
	for(unsigned int i = 0; i < elementsStack.size(); ++i)
		elementsStack.pop();
}

bool XmlParser::isElementOnTopStack(XmlElement* _element){
	if (elementsStack.size() == 0)
		return false;

	if (elementsStack.top()->getName() == _element->getName())
		return true;

	return false;
}

bool XmlParser::isSourceComplete(){
	return (source == sourceEnd);
}

bool XmlParser::testSymbol(const char& symbol){
	if (*source == symbol){
		return true;
	}

	return false;
}

bool XmlParser::testSymbolAndNextPosition(const char& symbol){
	if (testSymbol(symbol)){
		nextPosition();
		return true;
	}

	return false;
}

void XmlParser::nextPosition(){
	if (source == sourceEnd)
		throw ExceptionXml_UnexpectedEndOfInputData(stringNumber, columnNumber);

	if (testSymbol('\n')){
		prevColumnNumber = columnNumber;
		prevStringNumber = stringNumber;

		columnNumber = 0;
		++stringNumber;
	}
	else{
		prevColumnNumber = columnNumber;
		prevStringNumber = stringNumber;
		++columnNumber;
	}

	++source;
}

void XmlParser::prevPosition(){
	--source;
	columnNumber = prevColumnNumber;
	stringNumber = prevStringNumber;
}

bool XmlParser::isBadNameAttributeSymbol(const char& symbol){
	const char BAD_NAME_SYMBOLS_RANGE_1_END = 0x2f;
	const char BAD_NAME_SYMBOLS_RANGE_2_START = 0x3a;
	const char BAD_NAME_SYMBOLS_RANGE_2_END = 0x3f;
	const char BAD_NAME_SYMBOLS_3 = '\\';

	if ((symbol <= BAD_NAME_SYMBOLS_RANGE_1_END) ||
		((symbol >= BAD_NAME_SYMBOLS_RANGE_2_START) && (symbol <= BAD_NAME_SYMBOLS_RANGE_2_END)) ||
		(symbol == BAD_NAME_SYMBOLS_3))
		return true;

	return false;
}

string XmlParser::parseName(){
	string _name("");

	while (!isBadNameAttributeSymbol(*source)){
		_name += *source;
		nextPosition();
	}

	return _name;
}

pair<string, string> XmlParser::parseAttribute(){
	string _name("");
	string _value("");

	if (isBadNameAttributeSymbol(*source))
		throw ExceptionXml_ExtraCharactersInElementAttributeNames(stringNumber, columnNumber);

	while (!testSymbol('=')){
		_name += *source;
		nextPosition();
	}

	if (_name == "")
		throw ExceptionXml_NoAttributeName(stringNumber, columnNumber);

	nextPosition();

	if (!testSymbolAndNextPosition('"'))
		throw ExceptionXml_AttributeValueNotEnclosedInDoubleQuotes(stringNumber, columnNumber);

	while (!testSymbol('"')){
		_value += *source;
		nextPosition();
	}

	nextPosition();
	return make_pair(_name, _value);
}

void XmlParser::parseCommentAndSpace(){
	while(true){
		while (testSymbolAndNextPosition(' ') || testSymbolAndNextPosition('\v') || testSymbolAndNextPosition('\n')){}

		if (testSymbolAndNextPosition('<')){
			if (testSymbolAndNextPosition('!')){
				if (!testSymbolAndNextPosition('-'))
					throw ExceptionXml_InvalidComment(stringNumber, columnNumber);
				else
					if (!testSymbolAndNextPosition('-'))
						throw ExceptionXml_InvalidComment(stringNumber, columnNumber);
					else
						while(true){
							if (!testSymbolAndNextPosition('-'))
								nextPosition();
							else
								if (!testSymbolAndNextPosition('-'))
									nextPosition();
								else
									if (!testSymbolAndNextPosition('>'))
										nextPosition();
									else
										break;
						}
			}else{
				prevPosition();
				break;
			}
		}else
			break;
	}
}

void XmlParser::state1(){
//		cout << __FUNCTION__ << endl;
	try{
		parseCommentAndSpace();
	}
	catch(ExceptionXml_InvalidComment& e){
		throw e;
	}

	if (isSourceComplete()){
		stateFunc = nullptr;
		return;
	}

	if (!testSymbolAndNextPosition('<'))
		throw ExceptionXml_ExtraCharactersOutsideElement(stringNumber, columnNumber);

	if (testSymbolAndNextPosition('/'))
		stateFunc = &XmlParser::state2;
	else
		stateFunc = &XmlParser::state3;
}

void XmlParser::state2(){
//		cout << __FUNCTION__ << endl;
	if (elementsStack.size() == 0)
		throw ExceptionXml_UnexpectedShutdownElement(stringNumber, columnNumber);

	string name = parseName();

	if (!testSymbolAndNextPosition('>'))
		throw ExceptionXml_UnexpectedShutdownElement(stringNumber, columnNumber);

	if (name != elementsStack.top()->getName())
		throw ExceptionXml_UnexpectedShutdownElement(stringNumber, columnNumber);

	stateFunc = state7;
}

void XmlParser::state3(){
//		cout << __FUNCTION__ << endl;
	element = new XmlElement();
	element->setName(parseName());
	if (element->getName() == "")
		throw ExceptionXml_NoElementName(stringNumber, columnNumber);

	if (testSymbolAndNextPosition(' '))
		stateFunc = state5;
	else
		stateFunc = state4;
}

void XmlParser::state4(){
//		cout << __FUNCTION__ << endl;
	if (testSymbolAndNextPosition('/')){
		if (testSymbolAndNextPosition('>')){
			stateFunc = state7;
		}else{
			throw ExceptionXml_WrongEndOfElement(stringNumber, columnNumber);
		}
	}else{
		if (testSymbolAndNextPosition('>')){
			stateFunc = state6;
		}else{
			throw ExceptionXml_ExtraCharactersInElementAttributeNames(stringNumber, columnNumber);
		}
	}
}

void XmlParser::state5(){
//		cout << __FUNCTION__ << endl;
	while(true){
		try{
			parseCommentAndSpace();
		}
		catch(ExceptionXml_InvalidComment& e){
			throw e;
		}
		if (testSymbolAndNextPosition('>')){
			stateFunc = state6;
			break;
		}else{
			if (testSymbolAndNextPosition('/')){
				if (testSymbolAndNextPosition('>')){
					stateFunc = state7;
					break;
				}else{
					throw ExceptionXml_WrongEndOfElement(stringNumber, columnNumber);
				}
			}else{
				try{
					pair<string, string> attribute(parseAttribute());
					element->addAttribute(attribute.first, attribute.second);
				}
				catch(ExceptionXml_ExtraCharactersInElementAttributeNames& e){
					throw e;
				}
				catch(ExceptionXml_NoAttributeName& e){
					throw e;
				}
				catch(ExceptionXml_AttributeValueNotEnclosedInDoubleQuotes& e){
					throw e;
				}
			}
		}
	}
}

void XmlParser::state6(){
//		cout << __FUNCTION__ << endl;
	elementsStack.push(element);
	stateFunc = state1;
}

void XmlParser::state7(){
//		cout << __FUNCTION__ << endl;

	if (isElementOnTopStack(element)){
		elementsStack.pop();
	}

	if (elementsStack.size() == 0)
		elements.push_back(element);
	else
		elementsStack.top()->addElement(element);

	if (elementsStack.size() != 0)
		element = elementsStack.top();

	stateFunc = state1;
}

void XmlParser::print(){
	for (unsigned int i = 0; i < elements.size(); ++i)
		elements[i]->print();
}

void XmlParser::parseXml(char* sourceStart, char* _sourceEnd){
	if (sourceStart == nullptr)
		return;

	source = sourceStart;
	sourceEnd = _sourceEnd;

	columnNumber = 0;
	stringNumber = 0;
	prevColumnNumber = 0;
	prevStringNumber = 0;

	clearElements();
	clearElementsStack();

	stateFunc = &XmlParser::state1;

	try{
		while (stateFunc != nullptr)
			stateFunc();
	}
	catch(ExceptionXml_ExtraCharactersInElementAttributeNames& e){
		throw e;
	}
	catch(ExceptionXml_NoAttributeName& e){
		throw e;
	}
	catch(ExceptionXml_AttributeValueNotEnclosedInDoubleQuotes& e){
		throw e;
	}

	print();

	delete[] sourceStart;
}

char* XmlParser::source = nullptr;
char* XmlParser::sourceEnd = nullptr;
unsigned int XmlParser::columnNumber = 0;
unsigned int XmlParser::stringNumber = 0;
unsigned int XmlParser::prevColumnNumber = 0;
unsigned int XmlParser::prevStringNumber = 0;
XmlElement* XmlParser::element = nullptr;
vector<XmlElement*> XmlParser::elements;
stack<XmlElement*> XmlParser::elementsStack;
void (*XmlParser::stateFunc)();

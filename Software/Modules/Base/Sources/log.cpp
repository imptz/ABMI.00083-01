#include "log.h"
#include "config.h"

#include <syslog.h>

using namespace std;

const char* ExceptionLogStart::what() const throw(){
	return "Start log failed";
}
void Log::start(){
	try{
		int index = Config::getElementIndex("log");
		if (index == -1){
			syslog(LOG_INFO, "In the configuration file are not set log options module. Exit.");
			throw ExceptionLogStart();
		}

		string _ip;
		unsigned int _port = 0;

		XmlElement* xmlElement = Config::getElement(index);

		if (xmlElement->isAttributeExists("ip"))
			_ip = xmlElement->getAttributeValue("ip");
		else{
			syslog(LOG_INFO, "Log: ip attribute is not specified");
			throw ExceptionLogStart();
		}

		if (xmlElement->isAttributeExists("port"))
			_port = atoi(xmlElement->getAttributeValue("port").c_str());
		else{
			syslog(LOG_INFO, "Log: port attribute is not specified");
			throw ExceptionLogStart();
		}


		up(_ip.c_str(), _port);
		_thread = thread(&Log::threadFunc, this);
	}
	catch(const std::system_error& e) {
		throw ExceptionLogStart();
    }
}

void Log::join(){
	_thread.join();
}

void Log::putMessage(Message::TYPE type, const string& sender, const string& message){
//		cout << __FUNCTION__ << " sender: " << sender << endl;
	messagesMutex.lock();
//		cout << __FUNCTION__ << " in mutex " << " sender: " << sender << endl;
	messages.push(Message(type, sender, message));
	messagesMutex.unlock();
}

void Log::threadFunc(){
	while (true){
		sleep(1);
		setInstanceFlag();
	}
}

void Log::up(const char* _ip, unsigned int _port) throw(ExceptionLog){
}

void Log::down(){
}

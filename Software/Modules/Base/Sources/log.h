#ifndef LOG_H
#define LOG_H

#include "base.h"
#include <queue>
#include <mutex>
#include "singleton.h"

using namespace std;

class ExceptionLog : public std::exception{
};

class ExceptionLogStart : public ExceptionLog{
public:
	const char* what() const throw(){
		return "Start log failed";
	}
};

struct Message{
	enum TYPE{
		TYPE_ERROR,
		TYPE_INFO
	};
	TYPE type;
	string sender;
	string message;
	Message(TYPE _type, const string& _sender, const string& _message)	:
		type(_type), sender(_sender), message(_message)
	{}
};

class Log : public Base, public Singleton<Log>{
private:
	thread _thread;
	queue<Message> messages;
	mutex messagesMutex;

public:
	void start(){
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

	void join(){
		_thread.join();
	}

	void putMessage(Message::TYPE type, const string& sender, const string& message){
//		cout << __FUNCTION__ << " sender: " << sender << endl;
		messagesMutex.lock();
//		cout << __FUNCTION__ << " in mutex " << " sender: " << sender << endl;
		messages.push(Message(type, sender, message));
		messagesMutex.unlock();
	}

private:
	void threadFunc(){
		while (true){
			sleep(1);
			setInstanceFlag();
		}
	}

	void up(const char* _ip, unsigned int _port) throw(ExceptionLog){
	}

	void down(){
	}
};

#endif


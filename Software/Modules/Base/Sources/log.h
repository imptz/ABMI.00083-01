#ifndef LOG_H
#define LOG_H

#include "base.h"
#include "singleton.h"

#include <queue>
#include <mutex>
#include <thread>
#include <unistd.h>

using namespace std;

class ExceptionLog : public std::exception{
};

class ExceptionLogStart : public ExceptionLog{
public:
	const char* what() const throw();
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
	void start();
	void join();
	void putMessage(Message::TYPE type, const string& sender, const string& message);

private:
	void threadFunc();
	void up(const char* _ip, unsigned int _port) throw(ExceptionLog);
	void down();
};

#endif


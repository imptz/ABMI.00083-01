#ifndef STORAGE_H
#define STORAGE_H

#include <exception>
#include <string>
#include <mutex>

#include <mysql.h>

#include "singleton.h"

class ExceptionStorage : public std::exception{
};

class ExceptionStorageInit : public ExceptionStorage{
public:
	const char* what() const throw();
};

class ExceptionStoragePut : public ExceptionStorage{
public:
	const char* what() const throw();
};

class ExceptionStorageGet : public ExceptionStorage{
public:
	const char* what() const throw();
};

struct MessageType{
	enum TYPE{
		TYPE_INFO,
		TYPE_ERROR
	};

	static std::string typeToString(MessageType::TYPE& mt){
		switch(mt){
			case TYPE_INFO:
				return "INFO";
			case TYPE_ERROR:
				return "ERROR";
		}
	}

	static MessageType::TYPE stringToType(std::string& str){
		if (str == "INFO")
			return TYPE_INFO;

		if (str == "ERROR")
			return TYPE_ERROR;
	}
};

class Storage : public Singleton<Storage>{
private:
	std::mutex mutex;
	bool fInit;
	static const std::string DB_NAME;
	static const std::string DB_USER_NAME;
	static const std::string DB_USER_PASSWORD;
	static const std::string DB_ROOT_NAME;
	static const std::string DB_ROOT_PASSWORD;

	static const std::string QUERY_CREATE_DATABASE;
	static const std::string QUERY_DROP_DATABASE;

	MYSQL *mysql;

	bool createDatabase();

public:
	static const int LAST_MESSAGE_NUMBER = -1;
	Storage();
	~Storage();
	void init();
	void put(MessageType::TYPE messageType, char* message);
	std::string get(int start, unsigned int count);
};

#endif

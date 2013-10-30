#ifndef STORAGE_H
#define STORAGE_H

#include <exception>
#include <fstream>
#include <string>
#include <vector>

#include "singleton.h"

class ExceptionStorage : public std::exception{
protected:

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
	bool fInit;
	static const std::string STORAGE_FOLDER_NAME;
	static const std::string STORAGE_FILE_NAME;
	static const unsigned int MAX_LOG_FILE_SIZE = 1024 * 1024 * 10;

	std::ifstream inStream;
	std::ofstream outStream;

	std::vector<std::string> fileList;
	unsigned int lastFileNameNumber;

	void createLogsFolder();
	void createNextLogFile();

	long getFileSize(std::ofstream& _stream);

public:
	static const int LAST_MESSAGE_NUMBER = -1;
	Storage();
	~Storage();
	void init();
	void put(MessageType::TYPE messageType, char* message);
	std::string get(int start, unsigned int count);
};

#endif

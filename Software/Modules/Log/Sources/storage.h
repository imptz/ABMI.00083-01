#ifndef STORAGE_H
#define STORAGE_H

#include <exception>
#include <fstream>
#include <string>
#include <vector>

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

struct StorageFileHeader{
	static const unsigned int HEADER_STRING_LENGTH = 10;
	static const unsigned int MESSAGE_LENGTH = 1024;
	int messageCount;
	int writeMessageNumber;
	int realFileHeaderSize;
};

class Storage{
private:
	bool fInit;
	static const std::string STORAGE_FILE_NAME;

	static const unsigned int STORAGE_FILE_SIZE_MAX_LIMIT = StorageFileHeader::MESSAGE_LENGTH * 1024 * 10 + sizeof(StorageFileHeader);

	std::ifstream inStream;
	std::ofstream outStream;

	StorageFileHeader header;

	std::vector<std::string> messages;

	void readHeader();
	void writeHeader();

public:
	Storage();
	~Storage();
	void init();
	void put(MessageType::TYPE messageType, char* message);
};

#endif

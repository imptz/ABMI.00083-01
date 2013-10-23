#ifndef STORAGE_H
#define STORAGE_H

class ExceptionStorage : public std::exception{
};

class ExceptionStorageInit : public ExceptionStorage{
public:
	const char* what() const throw();
};

struct StorageFileHeader{
	static const unsigned int HEADER_STRING_LENGTH = 10;
	static const unsigned int MESSAGE_LENGTH = 1024;
	int messageLength;
	int messageCount;
	int writeMessageNumber;
};

class Storage{
private:
	bool fInit;
	static const std::string STORAGE_FILE_NAME;
	static const unsigned int STORAGE_FILE_SIZE_MAX_LIMIT = 1024 * 1024 * 10;

	std::fstream storageFile;

	StorageHeader header;
	std::vector<std::string> messages;

	void writeHeader();

public:
	Storage();
	~Storage();
	void init();
};

#endif

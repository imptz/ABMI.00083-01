#include "storage.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <time.h>

const char* ExceptionStorageInit::what() const throw(){
	return "Init storage init failed";
}

const char* ExceptionStoragePut::what() const throw(){
	return "Init storage put failed";
}

const std::string Storage::STORAGE_FILE_NAME = "messages";
					   
Storage::Storage()
	:	fInit(false){
}

Storage::~Storage(){
	inStream.close();
	outStream.close();
}

void Storage::readHeader(){	
	std::string buf;
	std::getline(inStream, buf);
	header.messageCount = std::stoi(buf);
	std::getline(inStream, buf);
	header.writeMessageNumber = std::stoi(buf);

	do{
		std::getline(inStream, buf);
	}while(buf != "");
}

void Storage::writeHeader(){	
	outStream.seekp(0, std::ios::beg);

	outStream << std::to_string(header.messageCount) << std::endl;
	outStream << std::to_string(header.writeMessageNumber) << std::endl;

	outStream.flush();
}

void Storage::init(){
	inStream.open(STORAGE_FILE_NAME, std::ifstream::binary);
	if (!inStream.good()){
		inStream.close();

		outStream.open(STORAGE_FILE_NAME, std::ios::out | std::fstream::binary);
		
		header.messageCount = 0;
		header.writeMessageNumber = 0;

		writeHeader();
		outStream.close();
		inStream.open(STORAGE_FILE_NAME, std::ifstream::binary);
	}	

	readHeader();
	if (!outStream.is_open())
		outStream.open(STORAGE_FILE_NAME, std::ios::out | std::ios::in | std::fstream::binary);

	std::cout << header.messageCount << " : " << header.writeMessageNumber << std::endl;

    fInit = true;
}

void Storage::put(MessageType::TYPE messageType, char* message){
	if (!fInit)
		throw ExceptionStoragePut();

	std::string msg;

	time_t dateTime;
	time(&dateTime);

	struct tm* dateTimeStruct = localtime(&dateTime);

	outStream << msg << dateTimeStruct->tm_year + 1900 << "." << dateTimeStruct->tm_mon + 1 << "." << dateTimeStruct->tm_mday << " " << dateTimeStruct->tm_hour << ":" << dateTimeStruct->tm_min << ":" << dateTimeStruct->tm_sec << " " << MessageType::typeToString(messageType) << " " << message << std::endl;
}

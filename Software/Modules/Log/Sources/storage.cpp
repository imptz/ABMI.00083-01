#include "storage.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <iomanip>  

const char* ExceptionStorageInit::what() const throw(){
	return "Storage init failed";
}

const char* ExceptionStoragePut::what() const throw(){
	return "Storage put failed";
}

const char* ExceptionStorageGet::what() const throw(){
	return "Storage get failed";
}

const std::string Storage::STORAGE_FOLDER_NAME = "Logs";
const std::string Storage::STORAGE_FILE_NAME = "log";
					   
Storage::Storage()
	:	fInit(false), lastFileNameNumber(0){
}

Storage::~Storage(){
	outStream.close();
}

void Storage::createLogsFolder(){
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(STORAGE_FOLDER_NAME.c_str())) == NULL){
    	if (mkdir(STORAGE_FOLDER_NAME.c_str(), 0760) == -1){
        	throw ExceptionStorageInit();
        }

        if((dp  = opendir(STORAGE_FOLDER_NAME.c_str())) == NULL)
        	throw ExceptionStorageInit();	
    }

    bool isEmpty = true;

    while ((dirp = readdir(dp)) != NULL){
    	std::string fileName(dirp->d_name);
    	if ((fileName != ".") && (fileName != "..")){
			isEmpty = false;
			break;
        }
    }

    closedir(dp);

	if (isEmpty){
		outStream.open(STORAGE_FOLDER_NAME + "/" + STORAGE_FILE_NAME + "0", std::ios::out | std::fstream::binary);
		outStream.close();
	}
}

void Storage::createNextLogFile(){
	if (outStream.is_open())
		outStream.close();

	++lastFileNameNumber;
	outStream.open(STORAGE_FOLDER_NAME + "/" + STORAGE_FILE_NAME + std::to_string(lastFileNameNumber), std::ios::out | std::fstream::binary);
	fileList.push_back(STORAGE_FILE_NAME + std::to_string(lastFileNameNumber));
}

void Storage::init(){
	createLogsFolder();

	try{
	    DIR *dp;
	    struct dirent *dirp;

	    if((dp  = opendir(STORAGE_FOLDER_NAME.c_str())) == NULL)
	        	throw ExceptionStorageInit();

	    while ((dirp = readdir(dp)) != NULL){
	    	std::string fileName(dirp->d_name);
	    	if ((fileName != ".") && (fileName != "..")){
	        	fileList.push_back(fileName);
	        }
	    }

	    closedir(dp);

		std::sort(fileList.begin(), fileList.end()); 

		if (fileList.size() == 0)
			throw ExceptionStorageInit();	

		lastFileNameNumber = stoi(fileList[fileList.size() - 1].substr(STORAGE_FILE_NAME.length(), std::string::npos));
		outStream.open(STORAGE_FOLDER_NAME + "/" + STORAGE_FILE_NAME + std::to_string(lastFileNameNumber), std::ios::in | std::ios::out | std::fstream::binary);
		outStream.seekp(0, std::ofstream::end);

	    fInit = true;
	}catch(const ExceptionStorage& e){
		throw e;
	}
}

long Storage::getFileSize(std::ofstream& _stream){
	auto curPos = _stream.tellp();
    std::cout << __FUNCTION__ << " cp " << curPos << std::endl;
    _stream.seekp(0, std::ofstream::end);
    auto fileSize = _stream.tellp();
    std::cout << __FUNCTION__ << "  " << fileSize << std::endl;
    _stream.seekp(curPos);
    return fileSize; 
}

void Storage::put(MessageType::TYPE messageType, char* message){
	if (!fInit)
		throw ExceptionStoragePut();

	if (getFileSize(outStream) >= MAX_LOG_FILE_SIZE)
		createNextLogFile();

	std::string msg;

	time_t dateTime;
	time(&dateTime);

	struct tm* dateTimeStruct = localtime(&dateTime);

	outStream << msg << 
		dateTimeStruct->tm_year + 1900 << 
		"." << std::setfill('0') <<
		std::setw(2) << dateTimeStruct->tm_mon + 1 << 
		"." << 
		std::setw(2) << dateTimeStruct->tm_mday << 
		"\t" << 
		std::setw(2) << dateTimeStruct->tm_hour << 
		":" << 
		std::setw(2) << dateTimeStruct->tm_min << 
		":" << 
		std::setw(2) << dateTimeStruct->tm_sec << 
		"\t" << 
		MessageType::typeToString(messageType) << 
		"\t" << 
		message << std::endl;

	outStream.flush();
}

std::string Storage::get(int start, unsigned int count){
	if (!fInit)
		throw ExceptionStorageGet();

	inStream.open(STORAGE_FOLDER_NAME + "/" + STORAGE_FILE_NAME + std::to_string(lastFileNameNumber), std::ios::in | std::fstream::binary);
	if (!inStream.good())
		throw ExceptionStorageGet();

	std::string result;
	std::string buf;
	do{
		std::getline(inStream, buf);
		result += buf + "\n";
	}while(buf != "");

	inStream.close();
	return result;
}

// if (!inStream.good()){

// void Storage::readHeader(){	
// 	std::string buf;
// 	std::getline(inStream, buf);
// 	header.messageCount = std::stoi(buf);
// 	std::getline(inStream, buf);
// 	header.writeMessageNumber = std::stoi(buf);

// 	do{
// 		std::getline(inStream, buf);
// 	}while(buf != "");
// }

// void Storage::writeHeader(){	
// 	outStream.seekp(0, std::ios::beg);

// 	outStream << std::to_string(header.messageCount) << std::endl;
// 	outStream << std::to_string(header.writeMessageNumber) << std::endl;

// 	outStream.flush();
// }

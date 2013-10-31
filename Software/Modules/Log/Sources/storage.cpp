#include "storage.h"

#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
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

const std::string Storage::DB_NAME = "uso";
const std::string Storage::DB_USER_NAME = "uso";
const std::string Storage::DB_USER_PASSWORD = "uso";
const std::string Storage::DB_ROOT_NAME = "root";
const std::string Storage::DB_ROOT_PASSWORD = "123456";

const std::string Storage::QUERY_CREATE_DATABASE = "create database " + DB_NAME + " character set utf8 collate utf8_general_ci";
const std::string Storage::QUERY_DROP_DATABASE = "drop database " + DB_NAME;

Storage::Storage()
	:	fInit(false), mysql(nullptr){
}

Storage::~Storage(){
	mysql_close(mysql);
}

void Storage::init(){
	mysql = mysql_init(nullptr);
	if (mysql == nullptr)
		throw ExceptionStorageInit();

	if (mysql_real_connect(mysql, "localhost", DB_USER_NAME.c_str(), DB_USER_PASSWORD.c_str(), DB_NAME.c_str(), 0, nullptr, 0) == nullptr){
		if (!createDatabase()){
			std::cout << __FUNCTION__ << " 1 " << mysql_error(mysql) << std::endl;
			throw ExceptionStorageInit();
		}
		else{
			if (mysql_real_connect(mysql, "localhost", DB_USER_NAME.c_str(), DB_USER_PASSWORD.c_str(), DB_NAME.c_str(), 0, nullptr, 0) == nullptr){
				std::cout << __FUNCTION__ << " 2 " << mysql_error(mysql) << std::endl;
				throw ExceptionStorageInit();
			}
		}
	}

	fInit = true;	
}

bool Storage::createDatabase(){
	mysql_close(mysql);
	if (mysql_real_connect(mysql, "localhost", DB_ROOT_NAME.c_str(), DB_ROOT_PASSWORD.c_str(), nullptr, 0, nullptr, 0) == nullptr){
		std::cout << __FUNCTION__ << " 1 " << mysql_error(mysql) << std::endl;
		mysql_close(mysql);
		return false;
	}

	if (mysql_query(mysql, QUERY_CREATE_DATABASE.c_str()) != 0){
	//if (mysql_query(mysql, QUERY_DROP_DATABASE.c_str()) != 0){
		std::cout << __FUNCTION__ << " 2 " << mysql_error(mysql) << std::endl;
		mysql_close(mysql);
		return false;
	}

	mysql_close(mysql);
	return true;
}

void Storage::put(MessageType::TYPE messageType, char* message){
	std::lock_guard<std::mutex> lock(mutex);
	if (!fInit){
		mutex.unlock();
		throw ExceptionStoragePut();
	}

	// MYSQL_RES *result;
	// MYSQL_ROW row;
	// int state;
	// state = mysql_query(connection, "SELECT * FROM stations");
	// if (state != 0){
	// 	std::cout << __FUNCTION__ << " query error " << mysql_error(connection) << std::endl;
	// 	return;
	// }

	// result = mysql_store_result(connection);
	// printf("Rows:%d\n",mysql_num_rows(result));

	// while ( ( row=mysql_fetch_row(result)) != NULL){
	// 	printf(" %s, %s\n", (row[0] ? row[0] : "NULL"), (row[1] ? row[1] : "NULL"));
	// }

	// mysql_free_result(result);

	// if (getFileSize(outStream) >= MAX_LOG_FILE_SIZE)
	// 	createNextLogFile();

	// std::string msg;

	// time_t dateTime;
	// time(&dateTime);

	// struct tm* dateTimeStruct = localtime(&dateTime);

	// outStream << msg << 
	// 	dateTimeStruct->tm_year + 1900 << 
	// 	"." << std::setfill('0') <<
	// 	std::setw(2) << dateTimeStruct->tm_mon + 1 << 
	// 	"." << 
	// 	std::setw(2) << dateTimeStruct->tm_mday << 
	// 	"\t" << 
	// 	std::setw(2) << dateTimeStruct->tm_hour << 
	// 	":" << 
	// 	std::setw(2) << dateTimeStruct->tm_min << 
	// 	":" << 
	// 	std::setw(2) << dateTimeStruct->tm_sec << 
	// 	"\t" << 
	// 	MessageType::typeToString(messageType) << 
	// 	"\t" << 
	// 	message << std::endl;

	// outStream.flush();
	mutex.unlock();
}

std::string Storage::get(int start, unsigned int count){
	std::lock_guard<std::mutex> lock(mutex);
	if (!fInit){
		mutex.unlock();
		throw ExceptionStorageGet();
	}

	// inStream.open(STORAGE_FOLDER_NAME + "/" + STORAGE_FILE_NAME + std::to_string(lastFileNameNumber), std::ios::in | std::fstream::binary);
	// if (!inStream.good()){
	// 	mutex.unlock();
	// 	throw ExceptionStorageGet();
	// }

	std::string result;
	// std::string buf;
	// do{
	// 	std::getline(inStream, buf);
	// 	result += buf + "\n";
	// }while(buf != "");

	// inStream.close();
	mutex.unlock();
	return result;
}

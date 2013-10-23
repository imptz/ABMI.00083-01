#include "storage.h"

const std::string Storahe::STORAGE_FILE_MANE = "messages";

Storage::Storage()
	:	fInit(false){
}

~Storage::Storage(){
	storageFile.close();
}

std::fstream::pos_type Storage::getFileSize(std::fstream _stream){
    std::fstream::pos_type position = _stream.tellg();
    _stream.seekg(0, std::fstream::end);
	std::fstream::pos_type size = _stream.tellg();
	_stream.seekg(position, std::fstream::beg);

	return size;
}

void Storage::init(){
	storageFile.open(STORAGE_FILE_NAME, std::fstream::in | std::fstream::outs);

	if (!storageFile.good()){
		header.messageLength = MESSAGE_LENGTH;
		header.messageCount = 0;
		header.writeMessageNumber = 0;

		writeHeader();
	}

	std::ifstream::pos_type storageSize = getFileSize(storageFile);
	if (storageSize > STORAGE_FILE_SIZE_MAX_LIMIT)
		throw ExceptionStorageInit();		

	std::filebuf* pbuf = storageFile.rdbuf();
	pbuf->sgetn(result.pData, result.size);

    fInit = true;
}

void Storage::writeHeader(){
	storageFile xxxxxxxxxxxxxxxxxxxxxxxxxx
}

// Resource Resources::getResource(std::string name){
// 	if (!fInit)
// 		throw ExceptionResourcesNotFound();

// 	auto it = resourcesList.find(name);
//     if (it == resourcesList.end())
//     	throw ExceptionResourcesNotFound();


// 	std::ifstream resourceFile(resourcesDir + name, std::ifstream::in | std::ifstream::binary);
// 	if (!resourceFile.good())
// 		throw ExceptionResourcesNotFound();

// 	Resource result;
// 	result.size = resourcesList[name];
// 	result.pData = new char[result.size];

// 	std::filebuf* pbuf = resourceFile.rdbuf();

// 	pbuf->sgetn(result.pData, result.size);

// 	resourceFile.close();

// 	return result;
// }

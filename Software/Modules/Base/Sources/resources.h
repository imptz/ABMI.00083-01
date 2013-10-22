#ifndef RESOURCES_H
#define RESOURCES_H

#include "http.h"

#include <string>
#include <map>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <fstream>

class ExceptionResources : public std::exception{
};

class ExceptionResourcesInit : public ExceptionResources{
public:
	const char* what() const throw();
};

class ExceptionResourcesNotFound : public ExceptionResources{
public:
	const char* what() const throw();
};

struct Resource{
	char* pData;
	size_t size;

	Resource()
		:	pData(nullptr), size(0){		
	}

	~Resource(){
		if (pData != nullptr)
			delete[] pData;
	}
};

class Resources{
private:
	bool fInit;
	std::string resourcesDir;
	std::map<std::string, size_t> resourcesList;
	std::ifstream::pos_type getFileSize(std::string filename);

public:
	Resources();
	void init(std::string _resourcesDir);
	Resource getResource(std::string name);
};

#endif

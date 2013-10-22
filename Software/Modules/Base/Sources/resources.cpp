#include "resources.h"

const char* ExceptionResourcesInit::what() const throw(){
	return "Initialize failed";
}

const char* ExceptionResourcesNotFound::what() const throw(){
	return "Resource not found";
}

Resources::Resources()
	:	fInit(false){
}

std::ifstream::pos_type Resources::getFileSize(std::string filename)
{
    std::ifstream in(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    in.seekg(0, std::ifstream::end);

	std::ifstream::pos_type size = in.tellg();
	in.close();

    return size; 
}

void Resources::init(std::string _resourcesDir){
	resourcesDir = _resourcesDir;

    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(resourcesDir.c_str())) == NULL)
        throw ExceptionResourcesInit();

    while ((dirp = readdir(dp)) != NULL){
    	std::string fileName(dirp->d_name);
    	if ((fileName != ".") && (fileName != "..")){
        	resourcesList["/" + fileName] = getFileSize(resourcesDir + "/" + fileName);
        }
    }

    closedir(dp);
    fInit = true;
}

Resource Resources::getResource(std::string name){
	if (!fInit)
		throw ExceptionResourcesNotFound();

	auto it = resourcesList.find(name);
    if (it == resourcesList.end())
    	throw ExceptionResourcesNotFound();


	std::ifstream resourceFile(resourcesDir + name, std::ifstream::in | std::ifstream::binary);
	if (!resourceFile.good())
		throw ExceptionResourcesNotFound();

	Resource result;
	result.size = resourcesList[name];
	result.pData = new char[result.size];

	std::filebuf* pbuf = resourceFile.rdbuf();

	pbuf->sgetn(result.pData, result.size);

	resourceFile.close();

	return result;
}

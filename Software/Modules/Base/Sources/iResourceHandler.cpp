#include "iResourceHandler.h"

const char* ExceptionResponseResourceNotFound::what() const throw(){
	return "404 not found";
}

IResourceHandler::~IResourceHandler() {}

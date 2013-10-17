#ifndef IRESOURCE_HANDLER
#define IRESOURCE_HANDLER

#include "http.h"

#include <string>

class ExceptionResponse : public std::exception{
};

class ExceptionResponseResourceNotFound : public ExceptionResponse{
public:
	const char* what() const throw();
};

class IResourceHandler{
public:
	virtual ~IResourceHandler();
	virtual std::string webHandler(HttpRequest& request) = 0;
};

#endif

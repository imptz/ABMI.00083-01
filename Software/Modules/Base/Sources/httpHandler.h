#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <thread>
#include "threadPool.h"
#include "http.h"
#include "iResourceHandler.h"
#include "resources.h"

class ErrorPage{
private:
	static const char* ERROR_PAGE_FIRST;
	static const char* ERROR_PAGE_SECOND;

public:
	static std::string getErrorPage(std::string errorInfo){
		return ERROR_PAGE_FIRST + errorInfo + ERROR_PAGE_SECOND;
	}
};


class HttpHandler : public IThread{
private:
	int sock;
	IResourceHandler* resourceHandler;
	Resources resources;

public:
	HttpHandler(int _sock, IResourceHandler* _resourceHandler);
	virtual ~HttpHandler();
	virtual void threadFunc();
};

#endif

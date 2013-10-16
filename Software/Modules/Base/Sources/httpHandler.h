#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <thread>
#include "threadPool.h"
#include "http.h"
#include "iResourceHandler.h"

class HttpHandler : public IThread{
private:
	int sock;
	IResourceHandler* resourceHandler;

public:
	HttpHandler(int _sock, IResourceHandler* _resourceHandler);
	virtual ~HttpHandler();
	virtual void threadFunc();
};

#endif

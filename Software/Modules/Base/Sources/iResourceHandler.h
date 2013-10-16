#ifndef IRESOURCE_HANDLER
#define IRESOURCE_HANDLER

#include <string>

class IResourceHandler{
public:
	virtual ~IResourceHandler();
	virtual std::string handler(std::string& resourcePath) = 0;
};

#endif

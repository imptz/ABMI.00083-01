#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <map>

class ExceptionHttp : public std::exception{
};

class ExceptionHttpFormat : public ExceptionHttp{
public:
	const char* what() const throw();
};

class ExceptionHttpRequestType : public ExceptionHttp{
public:
	const char* what() const throw();
};

class ExceptionHttpRequestResourcePath : public ExceptionHttp{
public:
	const char* what() const throw();
};

class ExceptionHttpRequestBody : public ExceptionHttp{
public:
	const char* what() const throw();
};

struct HttpRequest{
	std::string type;
	std::string resourcePath;
	std::map<std::string, std::string> headers;
	std::string body; 
};

class Http{
private:
	std::string::size_type parseRequestType(std::string& request, std::string& resultString);	
	std::string::size_type parseRequestResourcePath(std::string& request, std::string& resultString, std::string::size_type startPos = 0);
	void parseRequestBody(std::string& request, std::string& resultString);

public:
	HttpRequest parse(std::string& request);
};

#endif


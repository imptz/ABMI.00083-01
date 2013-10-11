#ifndef HTTP_H
#define HTTP_H

class ExceptionHttp : public std::exception{
};

class ExceptionHttpGormat : public ExceptionHttp{
public:
	const char* what() const throw(){
		return "Nepravilniy format Http zaprosa";
	}
};

struct HttpRequest{
	std::string type;
	std::string resourcePath;
	std::map<std::string, std::string> headers;
	std::string body; 
};

class Http{
public:
	static HttpRequest parse(std::string request){

	}
};

#endif


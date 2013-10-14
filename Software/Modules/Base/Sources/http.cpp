#include "http.h"

const char* ExceptionHttpFormat::what() const throw(){
	return "Nepravilniy format Http zaprosa";
}

const char* ExceptionHttpRequestType::what() const throw(){
	return "Nepravilniy tip metoda zaprosa";
}

const char* ExceptionHttpRequestResourcePath::what() const throw(){
	return "Nepravilniy puty resursa";
}

const char* ExceptionHttpRequestBody::what() const throw(){
	return "Net tela zaprosa ";
}

std::string::size_type Http::parseRequestType(std::string& request, std::string& resultString){
	std::string::size_type pos = request.find_first_of(" ");

	if (pos != std::string::npos){
		std::string testRequest = request.substr(0, pos);

		if (testRequest.compare("GET") == 0){
			resultString = "GET";
			return pos;
		}

		if (testRequest.compare("POST") == 0){
			resultString = "POST";
			return pos;
		}
	}
	
	throw ExceptionHttpRequestType();
}

std::string::size_type Http::parseRequestResourcePath(std::string& request, std::string& resultString, std::string::size_type startPos){
	std::string::size_type pos = request.find_first_of(" ", startPos + 1);

	if (pos != std::string::npos){
		resultString = request.substr(startPos, pos - startPos);
		return pos;
	}
	
	throw ExceptionHttpRequestResourcePath();
}

void Http::parseRequestBody(std::string& request, std::string& resultString){
	std::string::size_type pos = request.find_first_of("\r\n\r\n");

	if (pos != std::string::npos){
		resultString = request.substr(pos + 4);
		return;
	}
	
	throw ExceptionHttpRequestBody();
}

HttpRequest Http::parse(std::string& request){
	try{
		HttpRequest requestObject;

		std::string::size_type pos = parseRequestType(request, requestObject.type); 
		pos = parseRequestResourcePath(request, requestObject.resourcePath, pos); 
		parseRequestBody(request, requestObject.body); 
		return requestObject;
	}catch(ExceptionHttpRequestType& e){
		throw e;
	}catch(ExceptionHttpRequestResourcePath& e){
		throw e;
	}catch(ExceptionHttpRequestBody& e){
		throw e;
	}
}


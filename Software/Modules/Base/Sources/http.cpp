#include "http.h"

#include <iostream>

const char* ExceptionHttpFormat::what() const throw(){
	return "Nepravilniy format Http zaprosa";
}

const char* ExceptionHttpRequestType::what() const throw(){
	return "Nepravilniy tip metoda zaprosa";
}

const char* ExceptionHttpRequestResourcePath::what() const throw(){
	return "Nepravilniy puty resursa";
}

const char* ExceptionHttpRequestHeaders::what() const throw(){
	return "Nepravilnie zagolovki";
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
		resultString = request.substr(startPos + 1, pos - startPos - 1);

		return pos;
	}
	
	throw ExceptionHttpRequestResourcePath();
}

std::string::size_type Http::parseRequestHeaders(std::string& request, std::map<std::string, std::string>& headers){
	std::string::size_type pos = request.find("\r\n") + 2;

	if (pos != std::string::npos){
		std::string::size_type posSubStrEnd = 0;
		while(true){
			posSubStrEnd = request.find("\r\n", pos);
			std::string subStr = request.substr(pos, posSubStrEnd - pos);
			posSubStrEnd += 2;

			if (subStr.compare("") == 0)
				break;

			std::string::size_type posColon = subStr.find_first_of(":");
			std::string name = subStr.substr(0, posColon);
			std::string::size_type posEnd = subStr.find("\r\n", posColon);
			std::string value = subStr.substr(posColon + 2, posEnd - posColon - 2);
			headers[name] = value;
			pos = posSubStrEnd;
		}

		return posSubStrEnd;
	}
	
	throw ExceptionHttpRequestHeaders();
}

void Http::parseRequestBody(std::string& request, std::string& resultString, std::string::size_type startPos){
	resultString = request.substr(startPos, std::string::npos);
}

HttpRequest Http::parse(std::string& request){
	try{
		HttpRequest requestObject;

		std::string::size_type pos = parseRequestType(request, requestObject.type); 
		pos = parseRequestResourcePath(request, requestObject.resourcePath, pos); 
		pos = parseRequestHeaders(request, requestObject.headers); 
		parseRequestBody(request, requestObject.body, pos); 
		return requestObject;
	}catch(ExceptionHttpRequestType& e){
		throw e;
	}catch(ExceptionHttpRequestResourcePath& e){
		throw e;
	}catch(ExceptionHttpRequestBody& e){
		throw e;
	}
}


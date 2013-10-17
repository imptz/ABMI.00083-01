#include "httpHandler.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

const char* ErrorPage::ERROR_PAGE_FIRST = "<!DOCTYPE html><html lang=""ru""><head><meta charset=""UTF-8""/><title>Uso</title></head><body>";
const char* ErrorPage::ERROR_PAGE_SECOND = "</body></html>";

HttpHandler::HttpHandler(int _sock, IResourceHandler* _resourceHandler)
	:	sock(_sock), resourceHandler(_resourceHandler){
}

HttpHandler::~HttpHandler(){
}

void HttpHandler::threadFunc(){
	const unsigned int BUFFER_SIZE = 1024 * 1024 * 10;
	char buf[BUFFER_SIZE];

	int readCount = recv(sock, buf, BUFFER_SIZE, 0);
	if (readCount >= 0){
 		Http http;
 		std::string bufStr = std::string(buf);

 		std::string responseBody = "";
 		std::string response = "HTTP/1.1 200 OK\r\n\r\n";

 		HttpRequest request = http.parse(bufStr);
 		try{
			responseBody = resourceHandler->webHandler(request);
			response = response + responseBody;

		}catch(const ExceptionResponseResourceNotFound& e){
			response = "HTTP/1.1 404 not found\r\n\r\n" + ErrorPage::getErrorPage(e.what());
		}

		send(sock, response.c_str(), response.length(), 0);
	}

	close(sock);		
}

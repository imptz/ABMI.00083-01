#include "httpHandler.h"
#include "templatePage.h"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

const char* ErrorPage::ERROR_PAGE_FIRST = "<!DOCTYPE html><html lang=""ru""><head><meta charset=""UTF-8""/><title>Uso</title></head><body>";
const char* ErrorPage::ERROR_PAGE_SECOND = "</body></html>";

HttpHandler::HttpHandler(int _sock, IResourceHandler* _resourceHandler)
	:	sock(_sock), resourceHandler(_resourceHandler){
	try{
		resources.init(std::string("Resources"));
	}catch(ExceptionResourcesInit& e){
	}
}

HttpHandler::~HttpHandler(){
}

void HttpHandler::threadFunc(){
	const unsigned int BUFFER_SIZE = 1024 * 1024 * 10; // размер буфера должен динамически меняться от размера получаемых данных и размер должен быть известен resourceHandler-у
	char* buf = nullptr;

	try{
		buf = new char[BUFFER_SIZE];
		memset(buf, 0, BUFFER_SIZE);
		
		int readCount = recv(sock, buf, BUFFER_SIZE, 0);
		if (readCount > 0){
			try{
		 		Http http;
		 		std::string bufStr = std::string(buf);

//		 		std::cout << std::endl << bufStr << std::endl << std::endl;	

		 		std::string responseBody = "";
		 		std::string response = "HTTP/1.1 200 OK\r\n\r\n";

		 		HttpRequest request = http.parse(bufStr);
				try{
					response += std::string((resources.getResource(request.resourcePath)).pData);
				}catch(ExceptionResourcesNotFound& e){
			 		try{
						responseBody = resourceHandler->webHandler(request);
						response += responseBody;
					}catch(const ExceptionResponseResourceNotFound& e){
						std::string mainBlock(e.what() + std::string("\n"));
						response = "HTTP/1.1 404 not found\r\n\r\n" + TemplatePage::getPage(mainBlock);
					}
				}

				send(sock, response.c_str(), response.length(), 0);
			}catch(std::exception& e){
			}
		}

		delete[] buf;
	}catch(std::exception& e){
	}

	close(sock);		
}

#include "httpHandler.h"
#include "templatePage.h"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

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
	const unsigned int INIT_BUFFER_SIZE = 1024 * 1024 * 10;
	char* bufferStart = nullptr;

	try{
		bufferStart = new char[INIT_BUFFER_SIZE];
		memset(bufferStart, 0, INIT_BUFFER_SIZE);
		int readCount = recv(sock, bufferStart, INIT_BUFFER_SIZE, 0);
		if (readCount > 0){
			try{
		 		Http http;
		 		std::string bufStr = std::string(bufferStart);

		 		std::string responseBody = "";
		 		std::string response = "HTTP/1.1 200 OK\r\n\r\n";

		 		std::cout << __FUNCTION__ << std::endl << bufStr << std::endl;

		 		HttpRequest request = http.parse(bufStr);
		 		if ((request.resourcePath == "") || (request.resourcePath == "/")){
		 			request.resourcePath = "/index.html";
		 		}

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
	}catch(std::exception& e){
	}

	if (bufferStart != nullptr)
		delete[] bufferStart;

	close(sock);		
}

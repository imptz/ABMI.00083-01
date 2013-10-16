#include "httpHandler.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

HttpHandler::HttpHandler(int _sock, IResourceHandler* _resourceHandler)
	:	sock(_sock), resourceHandler(_resourceHandler){
}

HttpHandler::~HttpHandler(){
}

void HttpHandler::threadFunc(){
	char buf[1024];

	int readCount = recv(sock, buf, 1024, 0);
	if (readCount >= 0){
 		Http http;
 		HttpRequest request = http.parse(std::string(buf));
		std::string res = resourceHandler->handler(request);

//		send(sock, "HTTP/1.1 200 OK\r\n\r\nsssssssssssssssssssssssssssssssssssssss9\r\n", 100, 0);
		send(sock, res.c_str(), 100, 0);
	}

	close(sock);		
}

#include "webServer.h"
#include "httpHandler.h"

#include <iostream>

const char* ExceptionWebServerStart::what() const throw(){
	return "Start web server failed";
}

const char* ExceptionWebServerUp::what() const throw(){
	return "Web server up failed";
}

WebServer::WebServer()
	:	serverSocket(0)
{}

WebServer::~WebServer(){
	down();
}

void WebServer::start(IResourceHandler* _resourceHandler){
	try{
		resourceHandler = _resourceHandler;

		int index = Config::getElementIndex("server");
		if (index == -1){
			syslog(LOG_INFO, "In the configuration file are not set server options module. Exit.");
			throw ExceptionWebServerStart();
		}

		string _ip;
		unsigned int _port = 0;

		XmlElement* xmlElement = Config::getElement(index);

		if (xmlElement->isAttributeExists("ip"))
			_ip = xmlElement->getAttributeValue("ip");
		else{
			syslog(LOG_INFO, "WebServer: ip attribute is not specified");
			throw ExceptionWebServerStart();
		}

		if (xmlElement->isAttributeExists("port"))
			_port = atoi(xmlElement->getAttributeValue("port").c_str());
		else{
			syslog(LOG_INFO, "WebServer: port attribute is not specified");
			throw ExceptionWebServerStart();
		}

		up(_ip.c_str(), _port);
		_thread = thread(&WebServer::threadFunc, this);
	}
	catch(const ExceptionWebServer& e){
		throw ExceptionWebServerStart();
	}
	catch(const std::system_error& e) {
		down();
		throw ExceptionWebServerStart();
    }
}

void WebServer::join(){
	_thread.join();
}

void WebServer::threadFunc(){
	while (true){
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		setInstanceFlag();
		
		int clientSocket = accept(serverSocket, NULL, NULL);
		if (clientSocket >= 0){
			threadPool.initThread(new HttpHandler(clientSocket, resourceHandler));
		}
	}
}

void WebServer::up(const char* _ip, unsigned int _port) throw(ExceptionWebServer){
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == -1)
		throw ExceptionWebServerUp();

	int sockopt_on = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &sockopt_on, sizeof(int)) == -1)
		throw ExceptionWebServerUp();

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;//inet_addr(_ip);
	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0)
		throw ExceptionWebServerUp();

	listen(serverSocket, 10);
}

void WebServer::down(){
	close(serverSocket);
}

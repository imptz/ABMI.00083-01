#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <system_error>
#include <syslog.h>

#include "base.h"
#include "config.h"
#include "log.h"

using namespace std;

class ExceptionWebServer : public std::exception{
};

class ExceptionWebServerStart : public ExceptionWebServer{
public:
	const char* what() const throw(){
		return "Start web server failed";
	}
};

class ExceptionWebServerUp : public ExceptionWebServer{
public:
	const char* what() const throw(){
		return "Web server up failed";
	}
};

class WebServer : public Base{
private:
	thread _thread;

	int serverSocket;
	sockaddr_in serverAddr;

public:
	WebServer()	:
		serverSocket(0)
	{}

	~WebServer(){
		down();
	}

	void start(){
		try{
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

	void join(){
		_thread.join();
	}

private:
	void threadFunc(){
		while (true){
			sleep(1);
			setInstanceFlag();
			Log::getInstance().putMessage(Message::TYPE_INFO, "senderWebServer", "TestMessageWebServer");
		}
	}

	void up(const char* _ip, unsigned int _port) throw(ExceptionWebServer){
//		dispatcherSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
//		if (dispatcherSocket == -1)
//			throw ExceptionDispatcherClientDispatcherUp();
//
//		memset(&dispatcherAddr, 0, sizeof(dispatcherAddr));
//		dispatcherAddr.sin_family = AF_INET;
//		dispatcherAddr.sin_port = htons(_port);
//		dispatcherAddr.sin_addr.s_addr = inet_addr(_ip);
//
//		unsigned int moduleNameLength = strlen(moduleName);
//
//		sendPacket = new char[sizeof(DispatcherPacketHeader) + moduleNameLength];
//		if (sendPacket == nullptr){
//			close(dispatcherSocket);
//			throw ExceptionDispatcherClientDispatcherUp();
//		}
//
//		sendPacketHeader = reinterpret_cast<DispatcherPacketHeader*>(sendPacket);
//
//		sendPacketHeader->messageNumber = 0;
//		sendPacketHeader->hash = Crc32::calcCrc(reinterpret_cast<const unsigned char*>(moduleName), moduleNameLength);
//		memcpy(sendPacket + sizeof(DispatcherPacketHeader), moduleName, moduleNameLength);
//
//		sendPacketSize = moduleNameLength + sizeof(DispatcherPacketHeader);
	}

	void down(){
		close(serverSocket);
	}
};
#endif


#ifndef DISPATCHER_CLIENT_H
#define DISPATCHER_CLIENT_H

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
#include <dispatcherProtocol.h>
#include "config.h"
#include "log.h"
#include "crc32.h"

using namespace std;

class ExceptionDispatcherClient : public std::exception{
};

class ExceptionDispatcherClientStart : public ExceptionDispatcherClient{
public:
	const char* what() const throw(){
		return "Start dispatcher client failed";
	}
};

class ExceptionDispatcherClientDispatcherUp : public ExceptionDispatcherClient{
public:
	const char* what() const throw(){
		return "Dispatcher up failed";
	}
};

class DispatcherClient : public Base{
private:
	thread _thread;

	unsigned int timeOut;
	int dispatcherSocket;
	sockaddr_in dispatcherAddr;

	char* sendPacket;
	unsigned int sendPacketSize;
	DispatcherPacketHeader* sendPacketHeader;

public:
	DispatcherClient()	:
		timeOut(0), dispatcherSocket(0), sendPacket(nullptr), sendPacketSize(0), sendPacketHeader(nullptr)
	{}

	~DispatcherClient(){
		down();
	}

	void start(const char* moduleName){
		try{
			int index = Config::getElementIndex("dispatcher");
			if (index == -1){
				syslog(LOG_INFO, "In the configuration file are not set dispatcher options module. Exit.");
				throw ExceptionDispatcherClientStart();
			}

			string _ip;
			unsigned int _port = 0;

			XmlElement* xmlElement = Config::getElement(index);

			if (xmlElement->isAttributeExists("ip"))
				_ip = xmlElement->getAttributeValue("ip");
			else{
				syslog(LOG_INFO, "DispatcherClient: ip attribute is not specified");
				throw ExceptionDispatcherClientStart();
			}

			if (xmlElement->isAttributeExists("port"))
				_port = atoi(xmlElement->getAttributeValue("port").c_str());
			else{
				syslog(LOG_INFO, "DispatcherClient: port attribute is not specified");
				throw ExceptionDispatcherClientStart();
			}

			if (xmlElement->isAttributeExists("time_out"))
				timeOut = atoi(xmlElement->getAttributeValue("time_out").c_str());
			else{
				syslog(LOG_INFO, "DispatcherClient: time_out attribute is not specified");
				throw ExceptionDispatcherClientStart();
			}

			up(_ip.c_str(), _port, moduleName);
			_thread = thread(&DispatcherClient::threadFunc, this);
		}
		catch(const ExceptionDispatcherClient& e){
			throw ExceptionDispatcherClientStart();
		}
		catch(const std::system_error& e) {
			down();
			throw ExceptionDispatcherClientStart();
	    }
	}

	void join(){
		_thread.join();
	}

private:
	void threadFunc(){
		while (true){
			sleep(timeOut);
			setInstanceFlag();

			bool isAllFlagsSet = true;
			for (unsigned int i = 0; i < instancesFlags.size(); ++i)
				if (!instancesFlags[i])
					isAllFlagsSet = false;

			if (isAllFlagsSet)
				dispatcherSendPacket();

			clearAllInstancesFlags();
		}
	}

	void up(const char* _ip, unsigned int _port, const char* moduleName) throw(ExceptionDispatcherClient){
		dispatcherSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (dispatcherSocket == -1)
			throw ExceptionDispatcherClientDispatcherUp();

		memset(&dispatcherAddr, 0, sizeof(dispatcherAddr));
		dispatcherAddr.sin_family = AF_INET;
		dispatcherAddr.sin_port = htons(_port);
		dispatcherAddr.sin_addr.s_addr = inet_addr(_ip);

		unsigned int moduleNameLength = strlen(moduleName);

		sendPacket = new char[sizeof(DispatcherPacketHeader) + moduleNameLength];
		if (sendPacket == nullptr){
			close(dispatcherSocket);
			throw ExceptionDispatcherClientDispatcherUp();
		}

		sendPacketHeader = reinterpret_cast<DispatcherPacketHeader*>(sendPacket);

		sendPacketHeader->messageNumber = 0;
		sendPacketHeader->hash = Crc32::calcCrc(reinterpret_cast<const unsigned char*>(moduleName), moduleNameLength);
		memcpy(sendPacket + sizeof(DispatcherPacketHeader), moduleName, moduleNameLength);

		sendPacketSize = moduleNameLength + sizeof(DispatcherPacketHeader);
	}

	void down(){
		close(dispatcherSocket);
		if (sendPacket != nullptr)
			delete[] sendPacket;
	}

	void dispatcherSendPacket(){
		sendto(dispatcherSocket, sendPacket, sendPacketSize, 0, (struct sockaddr*) &dispatcherAddr, sizeof(dispatcherAddr));
		++(sendPacketHeader->messageNumber);
	}
};
#endif


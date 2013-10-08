#include "dispatcherClient.h"

#include <iostream>

using namespace std;

const char* ExceptionDispatcherClientStart::what() const throw(){
	return "Start dispatcher client failed";
}

const char* ExceptionDispatcherClientDispatcherUp::what() const throw(){
	return "Dispatcher up failed";
}

DispatcherClient::DispatcherClient()
	:	timeOut(0), dispatcherSocket(0), sendFrame(nullptr), sendFrameSize(0), sendFrameHeader(nullptr)
{}

DispatcherClient::~DispatcherClient(){
	down();
}

void DispatcherClient::start(const char* moduleName){
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

void DispatcherClient::join(){
	_thread.join();
}

void DispatcherClient::threadFunc(){
	while (true){
		sleep(timeOut);
		setInstanceFlag();

		bool isAllFlagsSet = true;
		for (unsigned int i = 0; i < instancesFlags.size(); ++i){
			std::cout << __FUNCTION__ << " instancesFlags[" << i << "] = " << instancesFlags[i] << std::endl;
			if (!instancesFlags[i])
				isAllFlagsSet = false;
		}

		if (isAllFlagsSet)
			dispatcherSendFrame();

		clearAllInstancesFlags();
	}
}

void DispatcherClient::up(const char* _ip, unsigned int _port, const char* moduleName) throw(ExceptionDispatcherClient){
	dispatcherSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (dispatcherSocket == -1)
		throw ExceptionDispatcherClientDispatcherUp();

	memset(&dispatcherAddr, 0, sizeof(dispatcherAddr));
	dispatcherAddr.sin_family = AF_INET;
	dispatcherAddr.sin_port = htons(_port);
	dispatcherAddr.sin_addr.s_addr = inet_addr(_ip);

	unsigned int moduleNameLength = strlen(moduleName);

	sendFrame = new char[sizeof(DispatcherFrameHeader) + moduleNameLength];
	if (sendFrame == nullptr){
		close(dispatcherSocket);
		throw ExceptionDispatcherClientDispatcherUp();
	}

	sendFrameHeader = reinterpret_cast<DispatcherFrameHeader*>(sendFrame);

	sendFrameHeader->messageNumber = 0;
	sendFrameHeader->hash = Crc32::calcCrc(reinterpret_cast<const unsigned char*>(moduleName), moduleNameLength);
	memcpy(sendFrame + sizeof(DispatcherFrameHeader), moduleName, moduleNameLength);

	sendFrameSize = moduleNameLength + sizeof(DispatcherFrameHeader);
}

void DispatcherClient::down(){
	close(dispatcherSocket);
	if (sendFrame != nullptr)
		delete[] sendFrame;
}

void DispatcherClient::dispatcherSendFrame(){
	sendto(dispatcherSocket, sendFrame, sendFrameSize, 0, (struct sockaddr*) &dispatcherAddr, sizeof(dispatcherAddr));
	++(sendFrameHeader->messageNumber);
}

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

class ExceptionDispatcherClient : public std::exception{
};

class ExceptionDispatcherClientStart : public ExceptionDispatcherClient{
public:
	const char* what() const throw();
};

class ExceptionDispatcherClientDispatcherUp : public ExceptionDispatcherClient{
public:
	const char* what() const throw();
};

class DispatcherClient : public Base, public Singleton<DispatcherClient>{
private:
	thread _thread;

	unsigned int timeOut;
	int dispatcherSocket;
	sockaddr_in dispatcherAddr;

	char* sendFrame;
	unsigned int sendFrameSize;
	DispatcherFrameHeader* sendFrameHeader;

public:
	DispatcherClient();
	~DispatcherClient();

	void start(const char* moduleName);
	void join();

private:
	void threadFunc();
	void up(const char* _ip, unsigned int _port, const char* moduleName) throw(ExceptionDispatcherClient);
	void down();
	void dispatcherSendFrame();
};

#endif

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

class ExceptionWebServer : public std::exception{
};

class ExceptionWebServerStart : public ExceptionWebServer{
public:
	const char* what() const throw();
};

class ExceptionWebServerUp : public ExceptionWebServer{
public:
	const char* what() const throw();
};

class WebServer : public Base, public Singleton<WebServer>{
private:
	thread _thread;

	int serverSocket;
	sockaddr_in serverAddr;

public:
	WebServer();
	~WebServer();
	void start();
	void join();

private:
	void threadFunc();
	void up(const char* _ip, unsigned int _port) throw(ExceptionWebServer);
	void down();
};

#endif

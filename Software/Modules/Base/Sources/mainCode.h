#ifndef MAIN_CODE_H
#define MAIN_CODE_H

#include <syslog.h>
#include "base.h"
#include "dispatcherClient.h"
#include "mainCode.h"
#include "webServer.h"
#include "cmd.h"
#include "config.h"
#include "log.h"

class ExceptionMainCode : public exception{
public:
	const char* what() const throw();
};

class MainCode : public Base{
protected:
	virtual void threadFunc() = 0;

public:
	virtual ~MainCode();

	void start(int argc, const char* argv[]) throw(ExceptionMainCode);
};

#endif

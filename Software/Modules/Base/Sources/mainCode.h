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

using namespace std;

class ExceptionMainCode : public exception{
public:
	const char* what() const throw(){
		return "Start main code failed";
	}
};

class MainCode : public Base{
protected:
	virtual void threadFunc() = 0;

public:
	virtual ~MainCode(){}

	void start(int argc, const char* argv[]) throw(ExceptionMainCode) {
		try{
			Cmd::init(argc, argv);

			string moduleName = "unknownModuleName";
			if (Cmd::isParameterExists(Cmd::CMD_NAME))
				moduleName = Cmd::getParameterValue(Cmd::CMD_NAME);

			openlog(moduleName.c_str(), LOG_PID, LOG_LOCAL0);

			if (Cmd::isParameterExists(Cmd::CMD_CONFIG_FILE))
				Config::init(Cmd::getParameterValue(Cmd::Cmd::CMD_CONFIG_FILE).c_str());
			else{
				syslog(LOG_INFO, "Not specified the name of the configuration file. Exit.");
				return;
			}


			DispatcherClient::getInstance().start(moduleName.c_str());
			Log::getInstance().start();
			WebServer::getInstance().start();

			threadFunc();

			WebServer::getInstance().join();
			Log::getInstance().join();
			DispatcherClient::getInstance().join();
		}
		catch(const ExceptionDispatcherClient& e){
			throw ExceptionMainCode();
		}
		catch(ExceptionXml& e){
			cout << "ExceptionConfig what = " << e.what() << " stingNumber = " << e.getStringNumber() << " columnNumber = " << e.getColumnNumber() << endl;
			throw ExceptionMainCode();
		}
	}
};
#endif


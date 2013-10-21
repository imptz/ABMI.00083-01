#include "mainCode.h"

#include <iostream>

using namespace std;

const char* ExceptionMainCode::what() const throw(){
	return "Start main code failed";
}

MainCode::~MainCode(){}

void MainCode::start(int argc, const char* argv[]) throw(ExceptionMainCode){
	try{
		Cmd::init(argc, argv);

		string moduleName = "uso::unknownModuleName";
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
		WebServer::getInstance().start(this);

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

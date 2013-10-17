#include <iostream>

#include "mainCode.h"
#include "log.h"
#include "threadPool.h"
#include "iResourceHandler.h"

using namespace std;

class MainCodeTest : public MainCode{
	virtual void threadFunc(){
		while(true){
			sleep(1);
			setInstanceFlag();
			Log::getInstance().putMessage(Message::TYPE_INFO, "senderMain", "TestMessageMain");
		}
	}
public:
	virtual ~MainCodeTest(){}
	virtual std::string webHandler(HttpRequest& request){
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::cout << "request.type: " << request.type << std::endl;
		std::cout << "request.resourcePath: " << request.resourcePath << std::endl;

		std::cout << "request.headers: " << std::endl;
		for(map<std::string, std::string>::const_iterator it = request.headers.begin(); it != request.headers.end(); ++it )
			std::cout << "  " << it->first << " : " << it->second << std::endl;

		std::cout << "request.body: " << std::endl << request.body;

		if (request.resourcePath != "/index.html")
			throw ExceptionResponseResourceNotFound();

		return "<!DOCTYPE html><html lang=""ru""><head><meta charset=""UTF-8""/><title>Uso</title></head><body>USO Base module</body></html>";
	}
};

int main(int argc, const char* argv[]) {
	MainCodeTest mainCode;

	try{
		mainCode.start(argc, argv);
	}
	catch(const ExceptionMainCode& e){
		cout << e.what() << endl;
	}

	return 0;
}


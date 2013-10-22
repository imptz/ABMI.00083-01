#include <iostream>
#include <sys/time.h>
#include <sstream>

#include "mainCode.h"
#include "log.h"
#include "threadPool.h"
#include "iResourceHandler.h"
#include "templatePage.h"

using namespace std;

class MainCodeTest : public MainCode{
private:
	virtual void threadFunc(){
		while(true){
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			setInstanceFlag();
		}
	}
	mutex webHandlerMutex;

public:
	virtual ~MainCodeTest(){}
	virtual std::string webHandler(HttpRequest& request){
		std::lock_guard<std::mutex> lock(webHandlerMutex);
		// struct timeval start;
		// gettimeofday(&start, NULL);

		// std::cout << __PRETTY_FUNCTION__ << " : " << start.tv_usec << std::endl;
		// std::cout << "request.type: " << request.type << std::endl;
		// std::cout << "request.resourcePath: " << request.resourcePath << std::endl;

		// std::cout << "request.headers: " << std::endl;
		// for(map<std::string, std::string>::const_iterator it = request.headers.begin(); it != request.headers.end(); ++it)
		// 	std::cout << "  " << it->first << " : " << it->second << std::endl;


		// std::cout << "request.body: " << std::endl << request.body;

		std::string mainBlock;

		if ((request.resourcePath == "/") || (request.resourcePath == "/index.html")){
			mainBlock = std::string("Resource: index.html журнал событий\n");
		}else{
			webHandlerMutex.unlock();
			throw ExceptionResponseResourceNotFound();
		}

		std::string result(TemplatePage::getPage(mainBlock));

		webHandlerMutex.unlock();
		return result;
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


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
	virtual std::string handler(std::string& resourcePath){
		return __FUNCTION__;
	}
};


class A : public IThread{
public:
	int f;
	A()
	:	f(0){
	}

	virtual void threadFunc(){
		while(f < 4){
			cout << __FUNCTION__ << " " << std::this_thread::get_id() << " " << f++ << endl;
			usleep(1000000);
		}
	}

	virtual ~A(){
		cout << __FUNCTION__ << " " << std::this_thread::get_id() <<  endl;
	}
};

// #include "http.h"

// int main(int argc, const char* argv[]){
// 	try{
// 		std::string test = "GET /index sdsd\r\n\r\ntelo zaprosa\nsssssssssssssssssssssssssssssss\nffffffffffffffffffffffffffff";

// 		Http http;
// 		HttpRequest request = http.parse(test);

// 		std::cout << "requestType = " << request.type << endl;
// 		std::cout << "requestResourcePath = " << request.resourcePath << endl;
// 		std::cout << "requestBody = " << request.body << endl;
// 	}catch(ExceptionHttpRequestType& e){
// 		std::cout << e.what() << std::endl;
// 	}catch(ExceptionHttpRequestResourcePath& e){
// 		std::cout << e.what() << std::endl;
// 	}catch(ExceptionHttpRequestBody& e){
// 		std::cout << e.what() << std::endl;
// 	}
// 	return 0;
// }

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


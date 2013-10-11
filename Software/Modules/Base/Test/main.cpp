#include <iostream>

#include "mainCode.h"
#include "log.h"
#include "threadPool.h"

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

int main(int argc, const char* argv[]) {
	MainCodeTest mainCode;

	A* a1 = new A();
	A* a2 = new A();

	try{
		ThreadPool tp;
		tp.initThread(a1);
		tp.initThread(a2);

		mainCode.start(argc, argv);
	}
	catch(const ExceptionMainCode& e){
		cout << e.what() << endl;
	}

	return 0;
}


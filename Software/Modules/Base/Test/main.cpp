#include <iostream>

#include "mainCode.h"
#include "log.h"

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

// int main(int argc, const char* argv[]) {
// 	MainCodeTest mainCode;

// 	try{
// 		mainCode.start(argc, argv);
// 	}
// 	catch(const ExceptionMainCode& e){
// 		cout << e.what() << endl;
// 	}

// 	return 0;
// }

#include "threadPool.h"

int main(int argc, const char* argv[]) {
	cout << "PoolTest::test == " << PoolTest::test() << endl;	
}

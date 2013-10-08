#ifndef CMD_H
#define CMD_H

#include <map>
#include <string>

using namespace std;

class Cmd{
private:
	static map<string, string> parameters;

	static bool isParametr(const char* str);

public:
	static void init(int argc, const char* argv[]);
	static bool isParameterExists(const string& key);
	static const string& getParameterValue(const string& key);

	static const string CMD_NAME;
	static const string CMD_CONFIG_FILE;
	static const string CMD_LAUNCH_DISPATCHER;
};

#endif


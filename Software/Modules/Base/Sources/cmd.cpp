#include "cmd.h"

using namespace std;

bool Cmd::isParametr(const char* str){
	if (str[0] == '-')
		return true;

	return false;
}

void Cmd::init(int argc, const char* argv[]){
	parameters.clear();
	if (argc > 1){
		for(int i = 1; i < argc - 1; ++i){
			if (isParametr(argv[i])){
				if (!isParametr(argv[i + 1]))
					parameters[argv[i] + 1] = argv[i + 1];
				else
					parameters[argv[i] + 1] = "";
			}
		}

		if (isParametr(argv[argc - 1]))
			parameters[argv[argc - 1] + 1] = "";
	}
}

bool Cmd::isParameterExists(const string& key){
	return (parameters.find(key) != parameters.end());
}

const string& Cmd::getParameterValue(const string& key){
	return parameters[key];
}

map<string, string> Cmd::parameters;
const string Cmd::CMD_NAME = "name";
const string Cmd::CMD_CONFIG_FILE = "c";
const string Cmd::CMD_LAUNCH_DISPATCHER = "launch_dispatcher";


#include <iostream>
#include <fstream>
#include <memory>
#include <sys/stat.h>
#include "FileUtils.h"
#include "JsonParser.h"

bool isFileExist(const std::string& name) {
	struct stat buffer;
	return stat(name.c_str(), &buffer) == 0;
}

std::string& readStringFromFile(const std::string& fileName,std::string& resieve){
	using namespace std;
	ifstream in(fileName);
	if (!in.is_open()) throw in;
	char ch;
	while (ch = in.get()) {
		if ((char)ch == EOF) {
			break;
		}
		resieve += ch;
	}
	return resieve;
}

JsonPrimitive* readJsonFromFile(const std::string& fileName){
	std::string raw;
	c32string model = c32string(readStringFromFile(fileName, raw));
	return xkball::jsonParser.parser(model);
}

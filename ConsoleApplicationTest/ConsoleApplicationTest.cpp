#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <cmath>
#include <codecvt>
#include <bitset>
#include <ctime>
#include "xstr.h"
#include "BitOut.h"
#include "c32string.h"
#include "FileUtils.h"
#include "JsonObj.h"
#include "JsonParser.h"
#include "member.h"
#include "Rule.h"

//习惯使然
#define sout std::cout
#define file const std::string

using EnumVSet = std::unordered_set<c32string, c32stringHash, c32stringEqual>;
using EnumPMap = std::unordered_map<c32string, EnumVSet, c32stringHash, c32stringEqual>;

namespace xkball {
	file fMembers = "members.json";
	file fProperties = "properties.json";
	file fRules = "rules.json";

	JsonArray* jMembers = nullptr;
	JsonObj* jProperties = nullptr;
	JsonObj* jRules = nullptr;
	std::vector<xkball::Member*> vMembers;
	std::vector<xkball::Member*> filteredMembers;
	int exCount = 0;

	EnumPMap enumPropertiesAvailable;
	int currentStep = 0;
}


int preInit();
int initMember();
int initRules();
void createPropertiesFile();
void createMembersFile();
void createRulseFile();
std::string& currentTime(std::string& rev);

using namespace std;
using namespace xkball;

int main() {

	//string self = "consoleApplicationTest.cpp";
	//string testJson = "test.json";
	//string modelJson = "scaffolding_common_button.json";
	//string outTxt = "out.txt";

	vector<string> steps{
		"解析/创建配置文件",
		"解析成员属性",
		"解析抽取规则",
		"执行抽取",
		"保存结果"
	};
	
	try {

		int i0 = preInit();
		if (i0) return i0;
		currentStep++;

		if (jMembers == nullptr) throw jMembers;
		if (jMembers->size() == 0) {
			sout << "请添加成员以供抽取";
			return 0;
		}
		initMember();
		currentStep++;

		initRules();
		exCount = jRules->getLongProperty(strings::count);
		if (exCount > filteredMembers.size()) throw "抽取数量大于可抽取数量";
		currentStep++;

		JsonObj result;
		XSTR random;
		long long seed = random.getSeed();
		result.addProperty(&strings::seed, new JsonString(*(new c32string(to_string(seed)))));
		int* resultIndex = new int[exCount];
		int exd = 0;
		JsonArray exa;
		while (exd<exCount){
			int r = random.nextLong(filteredMembers.size());
			bool b = true;
			for (int i = 0; i < exd; i++) {
				b &= resultIndex[i] != r;
			}
			if (b) {
				resultIndex[exd] = r;
				exd++;
			}
		}
		for (int i = 0; i < exCount; i++) {
			exa.add(*filteredMembers[resultIndex[i]]->save());
		}
		result.addProperty(strings::results, exa);
		currentStep++;

		sout << result;
		string outName("Result");
		outName += '-';
		currentTime(outName);
		outName += ".json";
		std::ofstream out(outName);
		if (!out.is_open()) throw out;
		out << result;
		out.close();
	}
	catch (c32string& err) {
		sout << err;
	}
	catch (...) {
		sout << "程序在 " << steps[currentStep] << " 阶段发生异常,请在xkball@qq.com或https://github.com/xkball/LearningCpp反馈问题";
	}

	//乐 不需要return
}

int preInit() {
	const std::locale utf8("en_US.UTF-8");
	//sout.imbue(utf8);
	//现在可以控制台显示中文
	system("chcp 65001");
	bool canContinue = true;
	if (!isFileExist(fProperties)) {
		canContinue = false;
		createPropertiesFile();
	}
	else {
		jProperties = readJsonFromFile(fProperties)->getAsJsonObj();
	}

	if (!isFileExist(fMembers)) {
		canContinue = false;
		if(jProperties != nullptr) createMembersFile();
	}
	else {
		jMembers = readJsonFromFile(fMembers)->getAsJsonArray();
	}

	if (!isFileExist(fRules)) {
		canContinue = false;
		createRulseFile();
	}
	else {
		jRules = readJsonFromFile(fRules)->getAsJsonObj();
	}

	if (!canContinue) {
		sout << "存在配置文件不存在 已经创建 请填写配置内容后重新运行程序";
		return currentStep + 10;
	}
	return 0;
}

int initMember() {
	JsonArray* enumPI = jProperties->getJsonArrProperty(strings::enumProperties)->getAsJsonArray();
	for (int i = 0; i < enumPI->size(); i++) {
		JsonObj* enumRaw = (*enumPI)[i]->getAsJsonObj();
		c32string* key = enumRaw->getStringProperty(strings::name);
		JsonArray* enumValue = enumRaw->getJsonArrProperty(strings::available_values)->getAsJsonArray();
		EnumVSet* values = new EnumVSet();
		for (int i = 0; i < enumValue->size(); i++) {
			c32string* value = (*enumValue)[i]->getAsJsonString()->getValue();
			values->insert(*value);
		}
		enumPropertiesAvailable.emplace(*key, *values);
	}
	for (int i = 0; i < jMembers->size(); i++) {
		vMembers.push_back(new Member(*(*jMembers)[i]->getAsJsonObj()));
	}
	return 0;
}

int initRules() {
	JsonArray* rawRules = jRules->getJsonArrProperty(strings::rules)->getAsJsonArray();
	for (int i = 0; i < rawRules->size(); i++) {
		unique_ptr<vector<c32string>> rule = unique_ptr<vector<c32string>>((*rawRules)[i]->getAsJsonString()->getValue()->split(' '));
		for (int j = 0; j < vMembers.size(); j++) {
			Member& tester = *vMembers[j];
			if (!testMember(rule.get(), tester)) tester.state = false;
		}
	}
	for (int j = 0; j < vMembers.size(); j++) {
		Member& tester = *vMembers[j];
		if (tester.state) filteredMembers.push_back(&tester);
		
	}
	return 0;
;}

void createPropertiesFile() {
	JsonObj json;
	json.addProperty(&strings::stringProperties,new JsonArray());
	json.addProperty(&strings::intProperties, new JsonArray);
	json.addProperty(&strings::doubleProperties, new JsonArray());
	json.addProperty(&strings::boolProperties, new JsonArray());
	json.addProperty(&strings::enumProperties, new JsonArray);
	std::ofstream out(fProperties);
	if (!out.is_open()) throw out;
	out << json;
	out.close();
}

void createMembersFile() {
	JsonArray arr;
	JsonObj* o = new JsonObj();
	if (jProperties != nullptr) {
		
		JsonArray* stringP = jProperties->getJsonArrProperty(strings::stringProperties)->getAsJsonArray();
		sout << stringP->size();
		for (int i = 0; i < stringP->size(); i++) {
			o->addProperty((*stringP)[i]->getAsJsonString()->getValue(), new JsonString());
		}
		JsonArray* intP = jProperties->getJsonArrProperty(strings::intProperties)->getAsJsonArray();
		for (int i = 0; i < intP->size(); i++) {
			o->addProperty((*intP)[i]->getAsJsonString()->getValue(), new JsonLong());
		}
		JsonArray* doubleP = jProperties->getJsonArrProperty(strings::doubleProperties)->getAsJsonArray();
		for (int i = 0; i < doubleP->size(); i++) {
			o->addProperty((*doubleP)[i]->getAsJsonString()->getValue(), new JsonDouble());
		}
		JsonArray* boolP = jProperties->getJsonArrProperty(strings::boolProperties)->getAsJsonArray();
		for (int i = 0; i < boolP->size(); i++) {
			o->addProperty((*boolP)[i]->getAsJsonString()->getValue(), new JsonBoolean());
		}
		JsonArray* enumP = jProperties->getJsonArrProperty(strings::enumProperties)->getAsJsonArray();
		for (int i = 0; i < enumP->size(); i++) {
			JsonObj* p = (*enumP)[i]->getAsJsonObj();
			o->addProperty(p->getStringProperty(strings::name), new JsonArray());
		}
		arr.add(*o);
	}
	std::ofstream out(fMembers);
	if (!out.is_open()) throw out;
	out << arr;
	out.close();
	delete o;
}

void createRulseFile() {
	JsonObj json;
	json.addProperty(&strings::count, new JsonLong());
	json.addProperty(&strings::rules, new JsonArray());
	std::ofstream out(fRules);
	if (!out.is_open()) throw out;
	out << json;
	out.close();
}

string& currentTime(string& rev) {
	time_t rawtime;
	struct tm ptminfo;

	time(&rawtime);
	localtime_s(&ptminfo,&rawtime);
	string buf;
	buf += to_string(ptminfo.tm_year+1900);
	buf += '-';
	buf += to_string(ptminfo.tm_mon + 1);
	buf += '-';
	buf += to_string(ptminfo.tm_mday);
	buf += '-';
	buf += to_string(ptminfo.tm_hour);
	buf += '-';
	buf += to_string(ptminfo.tm_min);
	buf += '-';
	buf += to_string(ptminfo.tm_sec);
	rev += buf;
	return rev;
}


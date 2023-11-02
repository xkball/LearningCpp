#include "member.h"

#define readJsonArr(key) jProperties -> getJsonArrProperty(key)-> getAsJsonArray()

#define readJsonData_p(jsonArr,toStorage,getter) \
	for (int i=0;i<jsonArr->size();i++){\
		c32string* key = (*jsonArr)[i]->getAsJsonString()->getValue();	\
		toStorage -> emplace(*key, *saved. getter (*key));\
	}

#define readJsonData(jsonArr, toStorage, getter) \
	for (int i = 0; i < jsonArr->size(); i++) {\
		c32string* key = (*jsonArr)[i]->getAsJsonString()->getValue();	\
		toStorage->emplace(*key, saved. getter (*key)); \
	}


using EnumVSet = std::unordered_set<c32string, c32stringHash, c32stringEqual>;
using EnumPMap = std::unordered_map<c32string, EnumVSet, c32stringHash, c32stringEqual>;

namespace xkball {
	extern JsonObj* jProperties;
	extern EnumPMap enumPropertiesAvailable;
}


xkball::Member::Member(){
	stringP = new StringProperties();
	intP = new IntProperties();
	doubleP = new DoubleProperties();
	boolP = new BoolProperties();
	enumP = new EnumProperties();
	state = true;
	saved_ = nullptr;
}

xkball::Member::Member(JsonObj& saved) {
	using namespace xkball;

	this->Member::Member();
	saved_ = new JsonObj(saved);
	JsonArray* strPI = readJsonArr(strings::stringProperties);
	readJsonData_p(strPI, stringP, getStringProperty);
	JsonArray* intPI = readJsonArr(strings::intProperties);
	readJsonData(intPI, intP, getLongProperty);
	JsonArray* doublePI = readJsonArr(strings::doubleProperties);
	readJsonData(doublePI, doubleP, getDoubleProperty);
	JsonArray* boolPI = readJsonArr(strings::boolProperties);
	readJsonData(boolPI, boolP, getBooleanProperty);
	JsonArray* enumPI = readJsonArr(strings::enumProperties);
	for (int i = 0; i < enumPI->size(); i++) {
		JsonObj* enumRaw = (*enumPI)[i]->getAsJsonObj();
		c32string* key = enumRaw->getStringProperty(strings::name);
		JsonArray* enumValue = enumRaw->getJsonArrProperty(strings::available_values)->getAsJsonArray();
		JsonArray* savedEnum = saved.getJsonArrProperty(*key)->getAsJsonArray();
		for (int i = 0; i < savedEnum->size(); i++) {
			c32string* value = (*savedEnum)[i]->getAsJsonString()->getValue();
			if (!(enumPropertiesAvailable.find(*key)->second).contains(*value)) std::cout << "存在未知枚举属性" << *key << " : " << *value;
			else enumP->emplace(*key, *value);
		}
	}
}

xkball::Member::~Member(){
	delete stringP;
	delete intP;
	delete doubleP;
	delete boolP;
	delete enumP;
	delete saved_;
}

JsonObj* xkball::Member::save(){
	return saved_;
}

#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "c32string.h"

namespace xkball {
	namespace json {
		extern const int JsonObject;
		extern const int JsonArray;
		extern const int JsonString;
		extern const int JsonLong;
		extern const int JsonDouble;
		extern const int JsonBoolean;
	}
}

class JsonObj;
class JsonArray;
class JsonString;
class JsonLong;
class JsonDouble;
class JsonBoolean;

class JsonPrimitive {
public:
	virtual int getType();
	virtual ~JsonPrimitive();
	JsonObj* getAsJsonObj();
	JsonArray* getAsJsonArray();
	JsonString* getAsJsonString();
	JsonLong* getAsJsonLong();
	JsonDouble* getAsJsonDouble();
	JsonBoolean* getAsJsonBoolean();
	friend std::ostream& operator<<(std::ostream& _Ostr, JsonPrimitive& arr);
};

using JsonMap = std::unordered_map<c32string, JsonPrimitive*, c32stringHash, c32stringEqual>;
using JsonVec = std::vector<JsonPrimitive*>;

class JsonArray :public JsonPrimitive {
public:
	virtual int getType() override;
	JsonPrimitive* operator[](int index);
	void add(JsonPrimitive& jsonPrimitive);
	//想不出怎么做remove啊 只能给个清空方法了
	void clear();
	int size();
	int getMemberType();
	friend std::ostream& operator<<(std::ostream& _Ostr, JsonArray& arr);

	JsonArray();
	//JsonArray(JsonArray&& _array);
	virtual ~JsonArray() override;
private:
	//仅可使用相同类型
	int type;
	JsonVec* jsonArray;
};


class JsonObj:public JsonPrimitive{
public:
	//读
	JsonArray* getJsonArrProperty(c32string& name);
	JsonObj* getJsonObjProperty(c32string& name);
	long getLongProperty(c32string& name);
	double getDoubleProperty(c32string& name);
	bool getBooleanProperty(c32string& name);
	c32string* getStringProperty(c32string& name);
	friend std::ostream& operator<<(std::ostream& _Ostr, JsonObj& obj);
	//std::vector<JsonEnrty>* getAllEntries();
	const JsonMap* getJsonMap();

	//写
	//void addProperty(c32string& name, JsonArray& jsonArray);
	//void addProperty(c32string& name, JsonObj& jsonObject);
	//void addProperty(c32string& name, JsonLong& jsonLong);
	//void addProperty(c32string& name, JsonDouble& jsonDouble);
	//void addProperty(c32string& name, JsonBoolean& jsonBoolean);
	void addProperty(c32string& name, JsonPrimitive& jsonPrimitive);
	void addProperty(c32string* name, JsonPrimitive* jsonPrimitive);
	void addProperty(c32string& name, long num);
	void addProperty(c32string& name, double num);
	void addProperty(c32string& name, bool boolean);
	void remove(c32string& name);

	//判断
	bool containsKey(c32string& name);
	virtual int getType() override;

	//构造
	JsonObj();
	JsonObj(JsonObj& toCopy);
	virtual ~JsonObj() override;

private:
	JsonMap* jsonElementMap;
	JsonPrimitive* checkedFind(c32string& key,int type);
};


class JsonString :public JsonPrimitive {
public:
	virtual int getType() override;
	c32string* getValue();
	void setValue(c32string& _string);
	friend std::ostream& operator<<(std::ostream& _Ostr, JsonString& string);

	JsonString();
	JsonString(c32string& initValue);
	virtual ~JsonString() override;
private:
	std::unique_ptr<c32string> jsonString;
};

class JsonLong :public JsonPrimitive {
public:
	virtual int getType() override;
	long getValue();
	void setValue(long value);
	friend std::ostream& operator<<(std::ostream& _Ostr, JsonLong& num);

	JsonLong();
	JsonLong(long initValue);
	virtual ~JsonLong() override;
private:
	long jsonLong;
};

class JsonDouble :public JsonPrimitive {
public:
	virtual int getType() override;
	double getValue();
	void setValue(double value);

	JsonDouble();
	JsonDouble(double initValue);
	virtual ~JsonDouble() override;
	friend std::ostream& operator<<(std::ostream& _Ostr, JsonDouble& num);
private:
	double jsonDouble;
};

class JsonBoolean :public JsonPrimitive {
public:
	virtual int getType() override;
	bool getValue();
	void setValue(bool value);

	JsonBoolean();
	JsonBoolean(bool initValue);
	virtual ~JsonBoolean() override;
	friend std::ostream& operator<<(std::ostream& _Ostr, JsonBoolean& num);
private:
	bool jsonBoolean;
};

class JsonFormat {
public:
	void operator++();
	void operator--();
	friend std::ostream& operator<<(std::ostream& _Ostr,const JsonFormat& f);
	bool enabled;

	JsonFormat();
private:
	int layer;
};


//class JsonEnrty {
//public:
//	JsonEnrty();
//	~JsonEnrty();
//
//	c32string& getName();
//	JsonPrimitive& getValue();
//private:
//	c32string name;
//	JsonPrimitive* jsonPrimitive;
//};

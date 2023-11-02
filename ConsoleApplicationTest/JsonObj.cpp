#include "JsonObj.h"

namespace xkball {
    namespace json {
        const int JsonObject = 0;
        const int JsonArray = 1;
        const int JsonString = 2;
        const int JsonLong = 3;
        const int JsonDouble = 4;
        const int JsonBoolean = 5;

        //不可多线程调用 不打算修
        JsonFormat formatUtil;
    }
}

JsonPrimitive::~JsonPrimitive() {

}

JsonObj* JsonPrimitive::getAsJsonObj(){
    if (this->getType() != xkball::json::JsonObject) throw this;
    return dynamic_cast<JsonObj*>(this);
}

JsonArray* JsonPrimitive::getAsJsonArray(){
    if (this->getType() != xkball::json::JsonArray) throw this;
    return dynamic_cast<JsonArray*>(this);
}

JsonString* JsonPrimitive::getAsJsonString(){
    if (this->getType() != xkball::json::JsonString) throw this;
    return dynamic_cast<JsonString*>(this);
}

JsonLong* JsonPrimitive::getAsJsonLong(){
    if (this->getType() != xkball::json::JsonLong) throw this;
    return dynamic_cast<JsonLong*>(this);
}

JsonDouble* JsonPrimitive::getAsJsonDouble(){
    if (this->getType() != xkball::json::JsonDouble) throw this;
    return dynamic_cast<JsonDouble*>(this);
}

JsonBoolean* JsonPrimitive::getAsJsonBoolean(){
    if (this->getType() != xkball::json::JsonBoolean) throw this;
    return dynamic_cast<JsonBoolean*>(this);
}

std::ostream& operator<<(std::ostream& out, JsonPrimitive& jp){
    int type = jp.getType();
    switch (type){
        case xkball::json::JsonObject:
            out << *jp.getAsJsonObj();
            break;
        case xkball::json::JsonArray:
            out << *jp.getAsJsonArray();
            break;
        case xkball::json::JsonString:
            out << *jp.getAsJsonString();
            break;
        case xkball::json::JsonLong:
            out << *jp.getAsJsonLong();
            break;
        case xkball::json::JsonDouble:
            out << *jp.getAsJsonDouble();
            break;
        case xkball::json::JsonBoolean:
            out << *jp.getAsJsonBoolean();
            break;
        default:
            out << xkball::json::formatUtil;
            break;
    }
    return out;
}

int JsonPrimitive::getType() {
    return -1;
}

//JsonObject
JsonObj::JsonObj() {
    jsonElementMap = new JsonMap();
}

JsonObj::JsonObj(JsonObj& toCopy) {
    jsonElementMap = new JsonMap();
    *jsonElementMap = *toCopy.jsonElementMap;
}

JsonObj::~JsonObj() {
    delete jsonElementMap;
}

JsonArray* JsonObj::getJsonArrProperty(c32string& name) {
    return dynamic_cast<JsonArray*>(checkedFind(name, xkball::json::JsonArray));
}

JsonObj* JsonObj::getJsonObjProperty(c32string& name) {
    return dynamic_cast<JsonObj*>(checkedFind(name, xkball::json::JsonObject));
}

long JsonObj::getLongProperty(c32string& name) {
    JsonLong* p = dynamic_cast<JsonLong*>(checkedFind(name,xkball::json::JsonLong));
    return p->getValue();
}

double JsonObj::getDoubleProperty(c32string& name) {
    JsonDouble* p = dynamic_cast<JsonDouble*>(checkedFind(name, xkball::json::JsonDouble));
    return p->getValue();
}


bool JsonObj::getBooleanProperty(c32string& name){
    JsonBoolean* p = dynamic_cast<JsonBoolean*>(checkedFind(name,xkball::json::JsonBoolean));
    return p->getValue();
}

c32string* JsonObj::getStringProperty(c32string& name) {
    JsonString* p = dynamic_cast<JsonString*>(checkedFind(name,xkball::json::JsonString));
    return p->getValue();
}

std::ostream& operator<<(std::ostream& out, JsonObj& obj){
    using namespace std;
    using namespace xkball;
    ++json::formatUtil;
    out << '{' << json::formatUtil;
    int i = 0;
    for (JsonMap::iterator it = obj.jsonElementMap->begin(); it != obj.jsonElementMap->end();it++) {
        i++;
        bool b = i < (obj.jsonElementMap->size());
        out <<'\"'<< it->first <<'\"' << ": " << *(it->second);
        if (b) {
            out << ',';
            out << json::formatUtil;
        }
    }
    --json::formatUtil;
    out << json::formatUtil;
    out << '}';
    return out;
}

const JsonMap* JsonObj::getJsonMap(){
    return jsonElementMap;
}

JsonPrimitive* JsonObj::checkedFind(c32string& key,int type){

    JsonMap::iterator got = jsonElementMap->find(key);
    if (got == jsonElementMap->end()) throw this;
    auto gotElement = got->second;
    if (gotElement->getType() != type) throw gotElement;
    return gotElement;
}

//不需要
//void JsonObj::addProperty(c32string& name, JsonArray& jsonArray) {
//    jsonElementMap->emplace(name,jsonArray);
//}
//
//void JsonObj::addProperty(c32string& name, JsonObj& jsonObject) {
//    jsonElementMap->emplace(name, jsonObject);
//}
//
//void JsonObj::addProperty(c32string& name, JsonLong& num) {
//    jsonElementMap->emplace(name,)
//}

void JsonObj::addProperty(c32string& name, JsonPrimitive& jsonPrimitive) {
    if (this == &jsonPrimitive) throw this;
    jsonElementMap->emplace(name, &jsonPrimitive);
}

void JsonObj::addProperty(c32string* name, JsonPrimitive* jsonPrimitive) {
    if (this == jsonPrimitive) throw this;
    jsonElementMap->emplace(*name, jsonPrimitive);
}

void JsonObj::addProperty(c32string& name, long num) {
    jsonElementMap->emplace(name, new JsonLong(num));
}

void JsonObj::addProperty(c32string& name, double num) {
    jsonElementMap->emplace(name, new JsonDouble(num));
}

void JsonObj::addProperty(c32string& name, bool boolean) {
    jsonElementMap->emplace(name, new JsonBoolean(boolean));
}

void JsonObj::remove(c32string& name) {
    jsonElementMap->erase(name);
}

bool JsonObj::containsKey(c32string& name) {
    return jsonElementMap->contains(name);
}

//JsonArray
JsonArray::JsonArray() {
    type = -1;
    jsonArray = new JsonVec();
}

JsonArray::~JsonArray() {
    delete jsonArray;
}

JsonPrimitive* JsonArray::operator[](int index){
    return (*jsonArray)[index];
}

void JsonArray::add(JsonPrimitive& jsonPrimitive) {
    if (this == &jsonPrimitive) throw this;
    if (type == -1) {
        type = jsonPrimitive.getType();
    }
    else if (type != jsonPrimitive.getType()) {
        int _type = jsonPrimitive.getType();
        if (type == xkball::json::JsonLong && _type == xkball::json::JsonDouble) {
            type = xkball::json::JsonDouble;
            for (int i = 0; i < size(); i++) {
                jsonArray->at(i) = new JsonDouble(jsonArray->at(i)->getAsJsonLong()->getValue());
            }
        }
        else if (_type == xkball::json::JsonLong && type == xkball::json::JsonDouble) {
            jsonArray->push_back(new JsonDouble(jsonPrimitive.getAsJsonLong()->getValue()));
            return;
        }
        else throw type;
    }
    jsonArray->push_back(&jsonPrimitive);
}

void JsonArray::clear() {
    jsonArray->clear();
}

int JsonArray::size() {
    return jsonArray->size();
}

int JsonArray::getMemberType() {
    return type;
}

std::ostream& operator<<(std::ostream& out, JsonArray& arr){
    using namespace std;
    using namespace xkball;
    ++json::formatUtil;
    out << '[' << json::formatUtil;
    for (int i = 0; i < arr.size();i++) {
        bool b = i < (arr.size() - 1);
        out << *arr[i];
        if (b) {
            out << ',';
        }
        out << json::formatUtil;
    }
    --json::formatUtil;
    //out << json::formatUtil;
    out << ']';
    return out;
}

//JsonString; 指针用法不知道有没有问题
JsonString::JsonString() {
    jsonString = std::unique_ptr<c32string>(new c32string());
}

JsonString::JsonString(c32string& initValue){
    jsonString = std::unique_ptr<c32string>(new c32string(initValue));
}

JsonString::~JsonString() {
    //delete jsonString;
}

c32string* JsonString::getValue() {
    return &*jsonString; 
}

void JsonString::setValue(c32string& value) {
    jsonString.reset(&value);
}

std::ostream& operator<<(std::ostream& out, JsonString& str) {
    out <<'\"' << * str.jsonString << '\"';
    return out;
}

//JsonLong
JsonLong::JsonLong() {
    jsonLong = 0;
}

JsonLong::JsonLong(long initValue) {
    jsonLong = initValue;
}

JsonLong::~JsonLong() {

}

void JsonLong::setValue(long value){
    jsonLong = value;
}

long JsonLong::getValue() {
    return jsonLong;
}

std::ostream& operator<<(std::ostream& out, JsonLong& num){
    out << num.getValue();
    return out;
}
//JsonDouble
JsonDouble::JsonDouble() {
    jsonDouble = 0;
}

JsonDouble::JsonDouble(double initValue) {
    jsonDouble = initValue;
}

JsonDouble::~JsonDouble() {

}

void JsonDouble::setValue(double value) {
    jsonDouble = value;
}

double JsonDouble::getValue() {
    return jsonDouble;
}

std::ostream& operator<<(std::ostream& _Ostr, JsonDouble& num) {
    _Ostr << num.jsonDouble;
    return _Ostr;
}

//JsonBool
JsonBoolean::JsonBoolean() {
    jsonBoolean = false;
}

JsonBoolean::JsonBoolean(bool initValue) {
    jsonBoolean = initValue;
}

JsonBoolean::~JsonBoolean() {

}

void JsonBoolean::setValue(bool value) {
    jsonBoolean = value;
}

bool JsonBoolean::getValue() {
    return jsonBoolean;
}

std::ostream& operator<<(std::ostream& _Ostr, JsonBoolean& num) {
    _Ostr << num.jsonBoolean;
    return _Ostr;
}

//类型识别
int JsonObj::getType() {
    return xkball::json::JsonObject;
}

int JsonArray::getType() {
    return xkball::json::JsonArray;
}

int JsonString::getType() {
    return xkball::json::JsonString;
}

int JsonLong::getType() {
    return xkball::json::JsonLong;
}

int JsonDouble::getType() {
    return xkball::json::JsonDouble;
}

int JsonBoolean::getType() {
    return xkball::json::JsonBoolean;
}

//int JsonPrimitive::getType(){
//    return -1;
//}

//输出格式化器
JsonFormat::JsonFormat() {
    layer = 0;
    enabled = true;
}

void JsonFormat::operator++() {
    layer++;
}

void JsonFormat::operator--() {
    layer--;
}

std::ostream& operator<<(std::ostream& out,const JsonFormat& f) {
    out << std::endl;
    if (f.enabled && f.layer>0) {
        for (int i = 0; i < f.layer; i++) {
            out << '\t';
        }
    }
    return out;
}
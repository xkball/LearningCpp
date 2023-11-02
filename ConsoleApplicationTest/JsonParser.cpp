#include "JsonParser.h"
#include "Math.h"
#include <string>

namespace xkball {
	//大括号
	const int BigParanthesesV = 1;
	//中括号
	const int BracketsV = 2;
	//小括号
	const int SmallParantheseV = 3;
	//冒号
	const int ColonV = 4;
	//双引号
	const int DoubleQuotesV = 5;
	//单引号
	const int Apostrophe = 6;
	//点
	const int DotV = 7;
	//逗号；
	const int CommaV = 8;

	const int EmptyV = -2;

	const std::string _true_ = "true";
	const c32string _true = c32string(_true_);

	const std::string _false_ = "false";
	const c32string _false = c32string(_false_);
}

xkball::JsonParser::JsonParser(){
	lastRead = 0;
	parserContext = nullptr;
}

xkball::JsonParser::~JsonParser(){
}

JsonPrimitive* xkball::JsonParser::parser(c32string& raw) {
	c32string rev{};
	parser(clearEmpty(raw,rev), 0);
	return parserContext;
}

void xkball::JsonParser::parser(const c32string& raw,int began){
	JsonPrimitive* result;
	int i = began;
	int startV = checkKeyValue(raw[i]);
	i++;
	if (startV == BigParanthesesV) {
		result = new JsonObj();
		i = parserObj(raw, i, *dynamic_cast<JsonObj*>(result));
	}
	else if (startV == BracketsV) {
		result = new JsonArray();
		i = parserArr(raw, i, *dynamic_cast<JsonArray*>(result));
	}
	else if (startV == DoubleQuotesV) {
		result = new JsonString();
		i = parserString(raw, i, *dynamic_cast<JsonString*>(result));
	}
	else {
		i = parserNum(raw, i-1);
		result = parserContext;
	}
	lastRead = i;
	parserContext = result;
}
//输入int为第一个字符,输出int为最后一个字符而非下一个的第一个字符!!!
//上面是错误的
//真正解析方式不可言传只可意会
int xkball::JsonParser::parserObj(const c32string& raw, int began, JsonObj& context){
	int i = began;
	while (checkKeyValue(raw[i]) == DoubleQuotesV){
		i++;
		JsonString* key = new JsonString();
		i = parserString(raw, i, *key);
		if (checkKeyValue(raw[i]) != ColonV) throw i;
		i++;
		parser(raw, i);
		JsonPrimitive* value = parserContext;
		i = lastRead;
		context.addProperty(*(key->getValue()), *value);
		if (checkKeyValue(raw[i]) == CommaV) i++;
	}
	if (checkKeyValue(raw[i] == BigParanthesesV)) i++;
	else throw i;
	return i;
}

int xkball::JsonParser::parserArr(const c32string& raw, int began, JsonArray& context){
	if (checkKeyValue(raw[began]) == BracketsV) return began+1;
	int i = began - 1;
	do {
		i++;
		parser(raw, i);
		JsonPrimitive* value = parserContext;
		i = lastRead;
		context.add(*value);
	} while (checkKeyValue(raw[i]) == CommaV);
	if (checkKeyValue(raw[i]) != BracketsV) throw i;
	return i+1;
}

int xkball::JsonParser::parserString(const c32string& raw, int began, JsonString& context){
	c32string* str = new c32string();
	int i = began;
	bool escape = false;
	while ( escape || checkKeyValue(raw[i]) != DoubleQuotesV){
		if ('\\' == (char)raw[i]) {
			escape = true;
		}
		else {
			escape = false;
			*str += raw[i];
		}
		i++;
	}
	context.setValue(*str);
	return i+1;
}

int xkball::JsonParser::parserNum(const c32string& raw, int began ){
	JsonPrimitive* context;
	c32string* str = new c32string();
	int i = began;
	while (isdigit(raw[i]) || '-' == raw[i] || checkKeyValue(raw[i]) == DotV) {
		*str += raw[i];
		i++;
	}
	if (_true == *str || _false == *str) {
		context = new JsonBoolean(_true == *str);
	}
	try {
		long l = parseLong(*str);
		context = new JsonLong(l);
	} catch(const int o){
		double d = parseDouble(*str);
		context = new JsonDouble(d);
	}
	parserContext = context;
	delete str;
	return i;
}

int xkball::JsonParser::checkKeyValue(const char32_t c){
	if (c & 0b11111111111111111111111100000000) return -1;
	char ch = c;
	if ('{' == ch || '}' == ch) return BigParanthesesV;
	if ('[' == ch || ']' == ch) return BracketsV;
	if ('(' == ch || ')' == ch) return SmallParantheseV;
	if (':' == ch) return ColonV;
	if ('"' == ch) return DoubleQuotesV;
	//暂不考虑字符
	if ('.' == ch) return DotV;
	if (',' == ch) return CommaV;
	if ('\n' == ch || '\t' == ch || ' ' == ch) return EmptyV;
	return -1;
}

//todo 临时用的厄厄方法
//还有字符串内转义和不忽略空格 要死
c32string& xkball::JsonParser::clearEmpty(c32string& raw, c32string& rev) {
	c32string buff(raw.length());
	bool inString = false;
	bool escape = false;
	for (int i = 0; i < raw.length(); i++) {
		char32_t ch = raw[i];
		if ('\"' == (char)ch && !escape) {
			inString = !inString;
		}
		if ('\\' == (char)ch) escape = true;
		else escape = false;
		
		if (checkKeyValue(ch) == EmptyV && !inString) continue;
		buff += raw[i];
	}
	//raw = std::move(buff);
	for (int i = 0; i < buff.length(); i++) {
		rev += buff[i];
	}	
	return rev;
}

namespace xkball {
	JsonParser jsonParser;
}
#pragma once
#include "JsonObj.h"

namespace xkball {

	//������
	extern const int BigParanthesesV;
	//������
	extern const int BracketsV;
	//С����
	extern const int SmallParantheseV;
	//ð��
	extern const int ColonV;
	//˫����
	extern const int DoubleQuotesV;
	//��
	extern const int DotV;

	extern const int EmptyV;


	class JsonParser {
	public:
		JsonParser();
		~JsonParser();

		JsonPrimitive* parser(c32string& raw);

	private:
		void parser(const c32string& raw, int began);
		int parserObj(const c32string& raw, int began, JsonObj& context);
		int parserArr(const c32string& raw, int began, JsonArray& context);
		int parserString(const c32string& raw, int began, JsonString& context);
		int parserNum(const c32string& raw, int began);
		int checkKeyValue(const char32_t ch);
		c32string& clearEmpty(c32string& raw,c32string& rev);

		int lastRead;
		JsonPrimitive* parserContext;
 	};

	extern JsonParser jsonParser;
}

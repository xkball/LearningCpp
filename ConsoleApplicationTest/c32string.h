#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>

class c32string {
public:
	c32string(int capacity = 20);
	c32string(char32_t* string,int _size);
	c32string(const std::string& utf8string);
	c32string(const c32string& copy);
	~c32string();
	c32string* append(char32_t* string,int toAdd);
	int length();

	std::size_t hash_code() const;

	char32_t operator[](const int index) const;
	c32string operator=(const c32string& toCopy);
	friend c32string& operator+=(c32string& src, char32_t ch);
	friend c32string& operator+=(c32string& src, c32string string);
	friend std::ostream& operator<<(std::ostream& _Ostr,const c32string& string);
	friend bool operator==(const c32string& left,const c32string& right);
	//c32string& operator=(c32string&& other) noexcept;
	friend c32string* substr(int start, int end, c32string& raw, c32string* rev);
	std::vector<c32string>* split(char32_t mark);

private:
	int size;
	int used;
	std::unique_ptr<char32_t[]> string;
	void changeStringSize(int _size);
};

struct c32stringHash {
	std::size_t operator()(const c32string& key) const {
		return key.hash_code();
	}
};

struct c32stringEqual {
	bool operator()(const c32string& c1, const c32string& c2) const {
		return c1 == c2;
	}
};

namespace xkball {

	#define estr(rawStr) extern c32string rawStr

	namespace strings {
		estr(stringProperties);
		estr(intProperties);
		estr(doubleProperties);
		estr(boolProperties);
		estr(enumProperties);
		estr(name);
		estr(count);
		estr(rules);
		estr(available_values);
		estr(include);
		estr(seed);
		estr(results);
		extern c32string true_;
		extern c32string false_;
	}
}
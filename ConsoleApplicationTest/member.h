#pragma once
#include <unordered_set>
#include "JsonObj.h"

//using namespace std;

namespace xkball {

	using StringProperties = std::unordered_map<c32string, c32string, c32stringHash, c32stringEqual>;
	using IntProperties = std::unordered_map<c32string, long, c32stringHash, c32stringEqual>;
	using DoubleProperties = std::unordered_map<c32string, double, c32stringHash, c32stringEqual>;
	using BoolProperties = std::unordered_map<c32string, bool, c32stringHash, c32stringEqual>;
	using EnumProperties = std::unordered_multimap<c32string, c32string, c32stringHash, c32stringEqual>;

	class Member {
	public:
		Member();
		Member(JsonObj& saved);
		~Member();
		JsonObj* save();

		StringProperties* stringP;
		IntProperties* intP;
		DoubleProperties* doubleP;
		BoolProperties* boolP;
		EnumProperties* enumP;
		bool state;
		JsonObj* saved_;
	};

}
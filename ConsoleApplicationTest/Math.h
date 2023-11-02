#pragma once
#include <string>
#include "c32string.h"

namespace xkball {

	const char musk4byte = 0b11110000;
	const char musk3byte = 0b11100000;
	const char musk2byte = 0b11000000;

	long unsignedRightMove(long value, int moveBits);
	int parseInt(std::string& string);
	long parseLong(c32string& string);
	double parseDouble(c32string& string);
}
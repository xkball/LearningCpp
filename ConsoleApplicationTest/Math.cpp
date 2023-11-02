#include "Math.h"

long xkball::unsignedRightMove(long value, int moveBits){
	using namespace std;
	unsigned long buf = value;
	buf >>= moveBits;
	return buf;
}

int xkball::parseInt(std::string& string){
	int result = 0;
	int size = string.size();
	for (int i = 0; i < size; i++) {
		if (isdigit(string[i])) {
			int buf = string[i] - 48;
			result += buf * pow(10, size - i - 1);
		}
		else {
			return 0;
		}
	}
	return result;
}

long xkball::parseLong(c32string& string){
	long result = 0;
	int size = string.length();
	char end = string[size - 1];
	if ('d' == end || 'D' == end || 'l' == end || 'L' == end) throw "not a int or long";
	//int dot = 0;
	bool isNegative = string[0] == '-';
	for (int i = isNegative; i < size; i++) {
		//if('.')
		if (isdigit(string[i])) {
			int buf = string[i] - 48;
			result += buf * pow(10, size - i - 1 - isNegative);
		}
		else {
			throw 0;
		}
	}
	return isNegative ? -result : result;
}

//既然能跑 就不要动了
double xkball::parseDouble(c32string& string){
	double result = 0.;
	int size = string.length();
	int dot = 0;
	if (string[0] == '-') {
		c32string buf;
		return -parseDouble(*substr(1, string.length() + 1, string, &buf));
	}
	for (int i = 0; i < size; i++) {
		if ('.' == (char)string[i] && dot == 0) {
			dot = i;
			result /= 10;
		}
		else if (isdigit(string[i])) {
			int buf = string[i] - 48;
			result += buf * pow(10, size - i - 1);
		}
		else {
			throw 0;
		}
	}
	result *= pow(10, dot-size+1 );
	return result;
}


#include <iostream>
#include<bitset>
#include "BitOut.h"


namespace xkball {

	constexpr int charBitL = sizeof(char) * 8;
	constexpr int intBitL = sizeof(int) * 8;
	constexpr int longBitL = sizeof(long) * 8;
	constexpr int longLongBitL = sizeof(long long) * 8;

}

using namespace std;
using namespace xkball;

bitout::bitout() {

}

void bitout::outAsChar(long l){
	//todo
}


void bitout::charOut(char ch) {
	bitset<charBitL> bitset2(ch);
	cout << bitset2 << std::endl;
}

void bitout::intOut(int i) {
	bitset<intBitL> bitset2(i);
	cout << bitset2 << std::endl;
}

void bitout::longOut(long l) {
	bitset<longBitL> bitset2(l);
	cout << bitset2 << std::endl;
}

void bitout::longLongOut(long long ll) {
	bitset<longLongBitL> bitset2(ll);
	cout << bitset2 << std::endl;	}


bitout& operator<<(bitout& bout, char ch) {
	bout.charOut(ch);
	return bout;
}

bitout& operator<<(bitout& bout, int i) {
	bout.intOut(i);
	return bout;
}

bitout& operator<<(bitout& bout, long l) {
	bout.longOut(l);
	return bout;
}

bitout& operator<<(bitout& bout, long long ll) {
	bout.longLongOut(ll);
	return bout;
}



namespace xkball{
	bitout bout{};
}



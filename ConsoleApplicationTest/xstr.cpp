
#include "xstr.h"
#include <ctime>
#include <chrono>
#include <iostream>

long long updateSeedUniqufiter();
long nanoTime();
static long long seedUniqufiter = 8682522807148012L;

XSTR::XSTR() {
	this -> XSTR::XSTR(updateSeedUniqufiter()* nanoTime());
}

XSTR::XSTR(long long seed_) {
	seed = seed_;
}

long XSTR::nextLong(int bound) {
	long last = seed ^ (seed << 21);
	unsigned long buff = last;
	buff ^= (buff >> 31);
	last = buff;
	//last ^= (last << 4);
	seed = last;
	long out = last % bound;
	return (out < 0) ? -out : out;
}

long long XSTR::getSeed(){
	return seed;
}

long XSTR::nextLong() {
	return nextLong(32);
}

long long updateSeedUniqufiter(){
	seedUniqufiter *= 181783497276652981L;
	return seedUniqufiter;
}

long nanoTime() {
	using namespace std::chrono;
	auto now = system_clock::now();
	auto time = now.time_since_epoch();
	//std::cout << time.count();
	return time.count();
}
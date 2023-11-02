#pragma once


class bitout {
public:
	friend bitout& operator<<(bitout& bout, char ch);
	friend bitout& operator<<(bitout& bout, int i);
	friend bitout& operator<<(bitout& bout, long l);
	friend bitout& operator<<(bitout& bout, long long ll);

	bitout();
	void outAsChar(long l);

private:
	void charOut(char ch);

	void intOut(int i);

	void longOut(long l);

	void longLongOut(long long ll);
};

namespace xkball {
	extern bitout bout;
}


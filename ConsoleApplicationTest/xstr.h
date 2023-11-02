#ifndef XSTR_H_

class XSTR {
	private:
		long long seed;

	public:
		XSTR();
		XSTR(long long);
		long nextLong();
		long nextLong(int bound);
		long long getSeed();
};
#endif // !XSTR_H_


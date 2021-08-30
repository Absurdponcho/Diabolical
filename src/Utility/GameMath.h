#pragma once

class FastRandom {
public:
	FastRandom();
	FastRandom(int Seed);
	void SetSeed(int NewSeed);
	inline int GetNextRandom();
	inline int GetNextRandom(int Low, int High);
private:
	int Seed;
};

class GlobalRandom {
	GlobalRandom() {};
	GlobalRandom(int Seed);
	// Don't Change Unless you want different LCG engine settings
	const int Modulus = 0x7FFFFFFF;
	const int Multiplier = 48271;
	const int Increment = 0;
public:
	static void Initialize();
	static void Initialize(int Seed);
	int operator()();
	void Discard(unsigned int z);
	static GlobalRandom* Get();


private:
	static GlobalRandom* Singleton;
	static int GlobalSeed;
};


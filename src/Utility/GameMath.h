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
	static const unsigned int Modulus = 0x7FFFFFFF;
	static const unsigned int Multiplier = 48271;
	static const unsigned int Increment = 0;
public:
	static void Initialize();
	static void Initialize(int Seed);
	static unsigned int Next();
	void Discard(unsigned int z);
	static GlobalRandom* Get();


private:
	static GlobalRandom* Singleton;
	static unsigned int GlobalSeed;
};


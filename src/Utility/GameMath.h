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


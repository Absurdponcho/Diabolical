#pragma once

class FastRandom {
public:
	FastRandom();
	FastRandom(int Seed);
	void SetSeed(int NewSeed);
	int GetNextRandom();
private:
	int Seed;
};


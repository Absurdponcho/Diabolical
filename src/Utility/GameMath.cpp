#include "GameMath.h"
#include <ctime>
#include <Windows.h>

FastRandom::FastRandom()
{
	this->Seed = _time32(nullptr);
	this->Seed ^= GetCurrentProcessId();
}

FastRandom::FastRandom(int Seed)
{
	this->Seed = Seed;
}

void FastRandom::SetSeed(int NewSeed)
{
	Seed = NewSeed;
}

int FastRandom::GetNextRandom()
{
	Seed = (214013 * Seed + 2531011);
	return (Seed >> 16) & 0x7FFF;
}

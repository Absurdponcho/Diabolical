#include "GameMath.h"
#include <ctime>
#include <Windows.h>
#include "..\GunGame.h"

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

inline int FastRandom::GetNextRandom(int Low, int High)
{
	Seed = (214013 * Seed + 2531011);
	return ((Seed >> 16) & 0x7FFF % ((High - Low) + 1) ) + Low;
}

GlobalRandom* GlobalRandom::Singleton;
unsigned int GlobalRandom::GlobalSeed = 0;

GlobalRandom::GlobalRandom(int Seed)
{
	GlobalSeed = Seed;
}

void GlobalRandom::Initialize()
{
	GlobalSeed = _time32(nullptr);
	GlobalSeed ^= GetCurrentProcessId();
}

void GlobalRandom::Initialize(int Seed)
{
	static bool bInitialized = false;
	Check(!bInitialized);
	bInitialized = true;
	Singleton = new GlobalRandom(Seed);
}

unsigned int GlobalRandom::Next()
{
	GlobalSeed = (Multiplier * GlobalSeed + Increment) % Modulus;
	return GlobalSeed;
}

void GlobalRandom::Discard(unsigned int z)
{
	for (z; z > 0; z--)
	{
		GlobalSeed = (Multiplier * GlobalSeed + Increment) % Modulus;
	}
}

GlobalRandom* GlobalRandom::Get()
{
	return Singleton;
}
